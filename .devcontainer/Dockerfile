FROM mcr.microsoft.com/vscode/devcontainers/rust:latest

# [Optional] Uncomment this section to install additional packages.
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

RUN rustup toolchain install nightly && rustup override set nightly
