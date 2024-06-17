## A Redis implementation in C++

### Implementation where we need to parse a request with specific schema

- We will get a request with schema

“+-----+------+-----+------+--------
| len | msg1 | len | msg2 | more...
+-----+------+-----+------+--------”

- As of now we will read this message, assuming len = 4bytes, will get message form the TCP request