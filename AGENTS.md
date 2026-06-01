# Local Codex Notes

This checkout tracks the `yix123/MinUI` fork and is used for building MinUI for `my355` on an Intel Mac.

## Local changes to preserve

- MinUI hibernate delay is changed in `workspace/all/common/api.c` from `120000` ms to `900000` ms so faux sleep powers off after 15 minutes instead of 2 minutes.
- `workspace/my355/cores/patches/fake-08.patch` was updated for current upstream `fake-08`: the patched load path calls `_vm->Step()` instead of the removed `_vm->UpdateAndDraw()`.
- `workspace/all/cores/patches/pokemini/0001-fix-resume-audio.patch` was updated for current upstream `PokeMini`: the old `source/PokeMini.c` `"LCD-"` to `"AUD-"` hunk is already upstream, so only the deferred unserialize patch remains.
- The `yix123/union-my355-toolchain` Dockerfile in `toolchains/my355-toolchain` supports Intel/x86_64 Docker hosts by enabling Debian `arm64` multiarch and installing the aarch64 cross compiler plus arm64 SDL/zlib/lzma/lzo/lz4/zstd runtime/dev packages.

## Rebuild commands

Build the Docker image if needed, then compile inside the Docker toolchain:

```sh
make -f makefile.toolchain toolchains/my355-toolchain/.build PLATFORM=my355
```

```sh
docker run --rm -v "$PWD/workspace:/root/workspace" my355-toolchain /bin/bash -lc 'cd /root/workspace && make'
```

Package only `my355` after a successful build:

```sh
make setup PLATFORMS=my355
docker run --rm -v "$PWD/workspace:/root/workspace" my355-toolchain /bin/bash -lc 'cd /root/workspace/my355 && make readmes'
make system PLATFORM=my355
make cores PLATFORM=my355
make special PLATFORMS=my355
make package PLATFORMS=my355
```

`make package` prints `commits.sh` warnings because this checkout only has the `my355` dependency trees populated, while the script tries to inspect every platform. The package still exits successfully.

## Current Git remotes

The MinUI checkout uses:

```text
origin   git@github.com:yix123/MinUI.git
upstream https://github.com/shauninman/MinUI.git
```

The nested toolchain checkout uses:

```text
origin   git@github.com:yix123/union-my355-toolchain.git
upstream https://github.com/shauninman/union-my355-toolchain/
```

If using HTTPS instead of SSH, replace the URLs with `https://github.com/yix123/...`.
