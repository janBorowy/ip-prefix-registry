# IPv4 prefix registry implementation

Fast implementation of most specific IPv4 subnet lookup. Relies
on [binary radix trie](https://en.wikipedia.org/wiki/Radix_tree),
which guarantees lookup within at most 32 trie edges - O(1).
Space complexity is O(N) for N registered prefixes.

## Building and running tests
```
git clone --recurse-submodules https://github.com/janBorowy/ip-prefix-registry
cd ip-prefix-registry
mkdir build
cd build
cmake -S .. -B .
cmake --build .
./ip-registry-test
```