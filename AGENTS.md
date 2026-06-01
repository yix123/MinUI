# Local Codex Notes

This checkout is a fork/worktree for building MinUI for `my355` on an Intel Mac.

## Local changes to preserve

- MinUI hibernate delay is changed in `workspace/all/common/api.c` from `120000` ms to `900000` ms so faux sleep powers off after 15 minutes instead of 2 minutes.
- `workspace/my355/cores/patches/fake-08.patch` was updated for current upstream `fake-08`: the patched load path calls `_vm->Step()` instead of the removed `_vm->UpdateAndDraw()`.
- `workspace/all/cores/patches/pokemini/0001-fix-resume-audio.patch` was updated for current upstream `PokeMini`: the old `source/PokeMini.c` `"LCD-"` to `"AUD-"` hunk is already upstream, so only the deferred unserialize patch remains.
- The `union-my355-toolchain` Dockerfile in `toolchains/my355-toolchain` was changed locally to support Intel/x86_64 Docker hosts by enabling Debian `arm64` multiarch and installing the aarch64 cross compiler plus arm64 SDL/zlib/lzma/lzo/lz4/zstd runtime/dev packages.

## Rebuild commands

Build inside the Docker toolchain:

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

## Git remotes

Upstream remotes initially point at Shaun Inman's repositories:

```sh
git remote add upstream https://github.com/shauninman/MinUI.git
git -C toolchains/my355-toolchain remote add upstream https://github.com/shauninman/union-my355-toolchain.git
```

After forking on GitHub, use personal remotes such as:

```sh
git remote set-url origin git@github.com:jakeliang/MinUI.git
git -C toolchains/my355-toolchain remote set-url origin git@github.com:jakeliang/union-my355-toolchain.git
```

If using HTTPS instead of SSH, replace the URLs with `https://github.com/jakeliang/...`.
