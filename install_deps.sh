#!/usr/bin/env bash

set -e

# Detect pkg manager
if command -v apt >/dev/null 2>&1; then
  PKG_MANAGER="apt"
elif command -v dnf >/dev/null 2>&1; then
  PKG_MANAGER="dnf"
elif command -v pacman >/dev/null 2>&1; then
  PKG_MANAGER="pacman"
else
  echo "Unsupported package manager, aborting..."
  exit 1
fi

install_apt() {
  sudo apt update

  sudo apt install -y \
    libsdl2-dev \
    libglew-dev \
    libsoil-dev \
    libglm-dev
}

install_dnf() {
  sudo dnf install -y \
    SDL2-devel \
    glew-devel \
    SOIL-devel \
    glm-devel
}

install_pacman() {
  sudo pacman -Sy --needed \
    sdl2 \
    glew \
    soil \
    glm
}

case $PKG_MANAGER in
  apt)
    install_apt
    ;;
  dnf)
    install_dnf
    ;;
  pacman)
    install_pacman
    ;;
esac

echo ""
echo "All dependencies installed successfully!"
