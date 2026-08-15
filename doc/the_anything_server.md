# The anything Server

yeah it's that good\
⠀⠀⡔⠉⠑⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣰⠋⠉⠉⠓⡆⠀⠀\
⠀⣸⠁⠀⠀⠀⠙⢦⡀⢸⡉⠓⠲⣄⡀⢀⡞⠀⠀⠀⠀⣀⣽⡤⠀\
⠀⡇⠀⠀⠀⠀⠀⠀⠙⣦⠷⠄⠀⠀⠙⠞⠀⠀⠀⠀⠀⠒⠚⡏⠁\
⠀⡇⠀⠀⠀⠀⣀⣀⡚⠓⠒⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀\
⠀⡇⠀⠀⠀⡎⠀⠀⠈⡆⠀⠀⠀⠀⠘⢄⣀⣀⡀⠀⠀⠀⣸⠁⠀\
⠀⠈⣇⠀⠐⡶⠖⣲⣶⡆⠀⠀⠀⠀⣶⣶⡒⠲⡒⠀⢀⡼⠁⠀⠀\
⠰⣖⠺⠧⢸⠁⠀⣿⣿⠇⠀⠀⠀⠀⣿⣿⠇⠀⡇⠀⠉⣩⠇⠀⠀\
⠀⠈⣳⠀⣨⢃⠀⠈⠉⠀⠒⠂⠀⠀⠈⠁⢀⠄⡡⠀⢼⡁⠀⠀⠀\
⠀⢰⣃⣈⣀⠁⠀⠀⠀⠦⠔⠓⠲⡲⠃⠀⠀⢈⣀⣀⣀⣹⡄⠀⠀\
⠀⠀⠀⠀⠀⠉⢳⠲⠤⢄⣀⣀⣀⣠⢶⠒⣏⠉⠀⠀⠀⠀⠀⠀⠀\
⠀⠀⠀⠀⠀⠀⠈⣳⠒⢲⠋⢱⠤⠧⠚⠋⠘⡆⠀⠀⠀⠀⠀⠀⠀\
⠀⠀⠀⠀⠀⠀⠘⠓⡆⠈⠒⠁⠀⠀⠀⠀⠀⢹⡀⠀⠀⠀⠀⠀⠀\
⠀⠀⠀⠀⠀⠀⠀⣼⣁⣀⣀⣀⣀⣀⣀⣀⣀⣀⡇⠀⠀⠀⠀⠀⠀


## How it works

The only thing that needs to be common to all protocols this server speaks, \
is the presence of a "start line". \
For example in http, the start line can be:
- `GET /index.html HTTP/1.1`
or
- `HTTP/1.1 200 OK`

these are easily recognizeable since they say what they are somewhere in it\
each protocol language module should have it's own regex start-line recognizer\
so that we can run it and figure out what library to use.





but how tf are we gonna make languages have their own module ??????
like http can have TLS \</3

maybe TLS is http reserved so we just add a security layer to transmissions that want TLS (or any other kind of security module)
