#!/usr/bin/env bash
set -euo pipefail

# Evita que variables de entorno del host (ej. clang/lld/compiler-rt) contaminen el toolchain devkitA64.
unset CC CXX CFLAGS CXXFLAGS CPPFLAGS LDFLAGS

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SWITCH_SRC="${ROOT_DIR}/switch"
DEVKITPRO_CMAKE="${DEVKITPRO:-/opt/devkitpro}/cmake/Switch.cmake"

usage() {
    cat <<EOF
Uso:
  $0 gen                 Compila el recolector PGO en build/switch-pgo-gen y lo copia a dist/final/pgo-collector/soulcalibur2.nro
  $0 final <dir_gcda>    Compila el ejecutable final PGO+LTO en build/switch-final usando los .gcda de <dir_gcda> y lo copia a dist/final/soulcalibur2.nro
EOF
    exit 1
}

if [[ $# -lt 1 ]]; then
    usage
fi

MODE="$1"
JOBS="$(nproc)"

case "${MODE}" in
    gen)
        BUILD_DIR="${ROOT_DIR}/build/switch-pgo-gen"
        OUT_DIR="${ROOT_DIR}/dist/final/pgo-collector"
        mkdir -p "${BUILD_DIR}" "${OUT_DIR}"
        cmake -S "${SWITCH_SRC}" -B "${BUILD_DIR}" \
            -DCMAKE_TOOLCHAIN_FILE="${DEVKITPRO_CMAKE}" \
            -DCMAKE_BUILD_TYPE=Release \
            -DSC2_ENABLE_LTO=OFF \
            -DSC2_PGO_GENERATE=ON
        cmake --build "${BUILD_DIR}" -j"${JOBS}"
        cp "${BUILD_DIR}/soulcalibur2.nro" "${OUT_DIR}/soulcalibur2.nro"
        echo "[PGO] Recolector listo en: ${OUT_DIR}/soulcalibur2.nro"
        ;;
    final)
        if [[ $# -lt 2 ]]; then
            usage
        fi
        GCDA_DIR="$(realpath "$2")"
        if [[ ! -d "${GCDA_DIR}" ]]; then
            echo "Error: no existe el directorio de datos PGO: ${GCDA_DIR}" >&2
            exit 1
        fi
        BUILD_DIR="${ROOT_DIR}/build/switch-final"
        OUT_DIR="${ROOT_DIR}/dist/final"
        mkdir -p "${BUILD_DIR}" "${OUT_DIR}"
        cmake -S "${SWITCH_SRC}" -B "${BUILD_DIR}" \
            -DCMAKE_TOOLCHAIN_FILE="${DEVKITPRO_CMAKE}" \
            -DCMAKE_BUILD_TYPE=Release \
            -DSC2_ENABLE_LTO=ON \
            -DSC2_PGO_GENERATE=OFF \
            -DSC2_PGO_USE="${GCDA_DIR}"
        cmake --build "${BUILD_DIR}" -j"${JOBS}"
        cp "${BUILD_DIR}/soulcalibur2.nro" "${OUT_DIR}/soulcalibur2.nro"
        echo "[PGO] Ejecutable final listo en: ${OUT_DIR}/soulcalibur2.nro"
        ;;
    *)
        usage
        ;;
esac
