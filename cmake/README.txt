general_balanced_tree_c
=======================

General Balanced trees (from Arne Andersson).
https://user.it.uu.se/~arneande/ps/gb.pdf

> We show that, in order to achieve efficient maintenance of a balanced binary search tree, no shape restriction other
> than a logarithmic height is required. The obtained class of trees, general balanced trees, may be maintained at a
> logarithmic amortized cost with no balance information stored in the nodes. Thus, in the case when amortized bounds
> are sufficient, there is no need for sophisticated balance criteria.
>
> The maintenance algorithms use partial rebuilding. This is important for certain applications, and has previously been
> used with weight-balanced trees. We show that the amortized cost incurred by general balanced trees is lower than what
> has been shown for weight-balanced trees.

This codebase makes minor changes to https://user.it.uu.se/~arneande/abs/gbimpl.html in order to support:

- C23 (it broke many of the old K&R pre-C89 concepts)
- CMake build system
- Creating a library (separating test CLI into a separate main.c in its own `target`)

(whilst retaining cross-platform support in C89)

---

## License

NOTE: 🤷 paper was from 26 years ago—before software licenses were popular—not sure what to do about this… the author also
hasn't published in decades… so I'll just license super permissively below and hopefully that's ok ツ

Licensed under any of:

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)
- CC0 license ([LICENSE-CC0](LICENSE-CC0) or <https://creativecommons.org/public-domain/cc0>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
licensed as above, without any additional terms or conditions.
