CMake project for vcpkg integration
===================================
[![License](https://img.shields.io/badge/license-Apache--2.0%20OR%20MIT-blue.svg)](https://opensource.org/licenses/Apache-2.0)

CMake has become the default 'standard' for cross-platform builds; popularly for C and C++.

This project aims to be a good starter for cross-platform third-party dependencies with CMake and vcpkg.

## vcpkg
[`vcpkg`](https://vcpkg.io) is an open-source cross-platform library package management system from [Microsoft](https://microsoft.com); targeting macOS, Linux, and Windows.

It's very popular, and has strong CMake integration. Henceforth, it is chosen for this project as an example of third-party library integration.

[Install vcpkg](https://vcpkg.io/en/getting-started.html), configure it with your system. Then run:

    [root]/vcpkg/vcpkg install zlib openssl

## Build

    $ mkdir 'cmake-build-debug' && cd "$_"
    $ cmake -DCMAKE_TOOLCHAIN_FILE='[root]/vcpkg/scripts/buildsystems/vcpkg.cmake' \
            -DCMAKE_BUILD_TYPE='Debug' \
            ..
    $ cmake --build .

---

This project originally was based upon the official `cmake\Help\guide\tutorial\Complete` at [`0fdfd6bf`](https://gitlab.kitware.com/cmake/cmake/-/tree/0fdfd6bf/Help/guide/tutorial/Complete).

---

## License

Licensed under either of

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE) or <https://www.apache.org/licenses/LICENSE-2.0>)
- MIT license ([LICENSE-MIT](LICENSE-MIT) or <https://opensource.org/licenses/MIT>)

at your option.

### Contribution

Unless you explicitly state otherwise, any contribution intentionally submitted
for inclusion in the work by you, as defined in the Apache-2.0 license, shall be
dual licensed as above, without any additional terms or conditions.
