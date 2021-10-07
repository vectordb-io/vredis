# vredis

use kv engine to simulate redis (hash, ttl)

```
hget key name: bruce
hget key age: 30
get kkk: vvv OK

get kkk: NotFound: kkk: ttl expired
get kkk, real value: vvv:1633613800

get kkk, real value: OK
```
