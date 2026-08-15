# testing the server

## sub domains
you can test sub domains easily by asking your browser to access something like: \
bruh.localhost:8080 \
(this only works with localhost tho, unless you setup a DNS)

## http header fields

(research by claude, have yet to confirm)

Here's the full list with the value type/format each header expects, based on the grammar rules the RFC defines for each field (Section 14). I'll use practical type labels (not C++ types specifically, but what shape of data you're parsing into) plus the expected format.

### General headers

| Header | Type | Format notes |
|---|---|---|
| `Cache-Control` | list of tokens/directives | comma-separated directives, some with `=value` (e.g. `max-age=3600`) — directive name is a token, value can be token or int depending on directive |
| `Connection` | list of tokens | comma-separated, e.g. `close`, `keep-alive`, or a header-name token |
| `Date` | date string | HTTP-date format, e.g. `Sun, 06 Nov 1994 08:49:37 GMT` |
| `Pragma` | list of tokens | e.g. `no-cache` |
| `Trailer` | list of tokens | header field names |
| `Transfer-Encoding` | list of tokens | e.g. `chunked` |
| `Upgrade` | list of tokens | product tokens, e.g. `HTTP/2.0` |
| `Via` | list of strings | `protocol pseudonym` pairs, comma-separated |
| `Warning` | list of structured strings | `code SP agent SP "text" [SP date]` — code is a 3-digit int |

### Request headers

| Header | Type | Format notes |
|---|---|---|
| `Accept` | list of media-type + qvalue | e.g. `text/html;q=0.9, */*;q=0.1` — qvalue is a float 0–1 |
| `Accept-Charset` | list of token + qvalue | same qvalue-list pattern |
| `Accept-Encoding` | list of token + qvalue | same pattern |
| `Accept-Language` | list of language-tag + qvalue | same pattern |
| `Authorization` | string (scheme + credentials) | e.g. `Basic <base64-string>` |
| `Expect` | token (usually one value) | e.g. `100-continue` |
| `From` | string | an email address (`mailbox` format from RFC 822) |
| `Host` | string | `hostname[:port]` — port, if present, is an int |
| `If-Match` | list of entity-tags, or `*` | quoted strings, comma-separated |
| `If-Modified-Since` | date string | HTTP-date format |
| `If-None-Match` | list of entity-tags, or `*` | same as If-Match |
| `If-Range` | entity-tag OR date string | either a quoted-string tag or an HTTP-date |
| `If-Unmodified-Since` | date string | HTTP-date format |
| `Max-Forwards` | integer | plain decimal integer |
| `Proxy-Authorization` | string | same shape as `Authorization` |
| `Range` | structured string | e.g. `bytes=500-999` — ints separated by `-`, comma-separated ranges allowed |
| `Referer` | string (URI) | absolute or relative URI |
| `TE` | list of token + qvalue | like Accept-Encoding, plus optional `;q=` |
| `User-Agent` | string | free-form product tokens + comments |

### Response headers

| Header | Type | Format notes |
|---|---|---|
| `Accept-Ranges` | list of tokens | e.g. `bytes`, or `none` |
| `Age` | integer | delta-seconds, non-negative decimal int |
| `ETag` | string (entity-tag) | quoted string, optionally prefixed `W/` |
| `Location` | string (URI) | absolute URI |
| `Proxy-Authenticate` | string | challenge string, similar shape to Authorization |
| `Retry-After` | date string OR integer | either HTTP-date, or delta-seconds (int) |
| `Server` | string | free-form product tokens + comments |
| `Vary` | list of tokens, or `*` | header field names, comma-separated |
| `WWW-Authenticate` | string | challenge string |

### Entity headers

| Header | Type | Format notes |
|---|---|---|
| `Allow` | list of tokens | HTTP method names, comma-separated |
| `Content-Encoding` | list of tokens | e.g. `gzip` |
| `Content-Language` | list of language-tags | e.g. `en-US` |
| `Content-Length` | integer | plain decimal integer, byte count — **the one you actually need first** |
| `Content-Location` | string (URI) | absolute or relative URI |
| `Content-MD5` | string (base64) | base64-encoded 128-bit MD5 digest |
| `Content-Range` | structured string | e.g. `bytes 0-499/1234` — ints and a `*` fallback |
| `Content-Type` | structured string | `type/subtype` + optional `;param=value` pairs, e.g. `text/html; charset=utf-8` |
| `Expires` | date string | HTTP-date format |
| `Last-Modified` | date string | HTTP-date format |

### Practical parsing note

At the byte level, **every single header field value is just a string** when it arrives over the socket — HTTP has no binary/typed encoding at all, everything is ASCII text. The "type" column above is really about *how you should interpret and convert* the string after you've extracted it, not how it arrives on the wire. So your parser's job in two stages:

1. Generic stage (same for all headers): split on the first `:`, trim whitespace → you get `(name: string, value: string)`
2. Per-header stage (specific to that header): convert that string into the type that's actually useful to your logic — `Content-Length` → `std::stoul(value)` into a `size_t`, `Date`/`If-Modified-Since` → parse into a `time_t`/`tm` struct, `Content-Type` → maybe split on `;` and store type/subtype/params separately, etc.

For a minimal server, you can genuinely just keep most headers as raw strings in a `map<string, string>` and only bother converting the handful you actually act on (`Content-Length` to an integer being the main one you can't avoid).
