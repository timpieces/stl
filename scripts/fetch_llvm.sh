#!/usr/bin/env bash
set -euo pipefail

DEST_DIR=${1:-third_party/llvm-project}
REPO_URL="https://github.com/llvm/llvm-project.git"

if [[ -d "${DEST_DIR}/.git" ]]; then
    echo "LLVM project already checked out at ${DEST_DIR}" >&2
    exit 0
fi

mkdir -p "$(dirname "${DEST_DIR}")"

git clone --depth=1 --branch main --single-branch "${REPO_URL}" "${DEST_DIR}"
