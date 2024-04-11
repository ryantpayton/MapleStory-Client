# [Choice] Swift version: 5.6-focal, 5.5, 5.4, 5.3, 5.2, 5.1, 4.2
ARG VARIANT=5.10-jammy
FROM swift:${VARIANT}

# [Option] Install zsh
ARG INSTALL_ZSH="true"
# [Option] Upgrade OS packages to their latest versions
ARG UPGRADE_PACKAGES="false"

# Install needed packages and setup non-root user. Use a separate RUN statement to add your own dependencies.
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID
COPY library-scripts/common-debian.sh /tmp/library-scripts/
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && /bin/bash /tmp/library-scripts/common-debian.sh "${INSTALL_ZSH}" "${USERNAME}" "${USER_UID}" "${USER_GID}" "${UPGRADE_PACKAGES}" "true" "true" \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* && rm -rf /tmp/library-scripts

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends apt-transport-https \
     ca-certificates \
     gnupg \
     software-properties-common \
     build-essential \
     cmake \
     autoconf \
     libtool \
     pkg-config \
     git \
     libopenal-dev libvorbis-dev libopusfile-dev libsndfile1-dev \
     xorg-dev \
     libgl1-mesa-dev