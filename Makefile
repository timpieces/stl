BUILD_DIR ?= build
CONFIGURE_FLAGS ?=
CONDA ?= conda
CONDA_ENV_NAME ?= stl

.PHONY: configure build test run clean llvm compile_commands env

configure:
	cmake -S . -B $(BUILD_DIR) $(CONFIGURE_FLAGS)
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

build: configure
	cmake --build $(BUILD_DIR)

run: build
	"$(BUILD_DIR)/stl_hello_tests"

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

llvm:
	scripts/fetch_llvm.sh

compile_commands: configure

env:
	$(CONDA) env update --name $(CONDA_ENV_NAME) --file environment.yml --prune

clean:
	rm -rf $(BUILD_DIR) compile_commands.json
