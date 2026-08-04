
Table of content:
1. **[HTTP format, basics](#http-format)**
2. **[Request format](#request-format)**
3. **[Response format](#response-format)**

# HTTP format

```
start-line CRLF
header-name: header-value CRLF
header-name: header-value CRLF
... CRLF
CRLF
[optional body]
```

After the start line is the hearder \
Once you encounter the double CRLF the header is done \
If you read anything after that isn't a start line, it's the body \
A request or response that has body **MUST** have one (cannot be both) of the following entries in its header:
```
Content-Length: value
Transfer-Encoding: value
```


# request format

## start line

# response format

## start line
