#!/bin/bash

oneTimeSetUp() {
    THIS_DIR=$(dirname "$0")
    source "$THIS_DIR/utils.sh"
}

testNoUnexpectedKeywordsOrder() {
    PROJECT_ROOT_DIR=$(GetProjectRootDir)
    QuietRun pushd "$PROJECT_ROOT_DIR"

    assertFalse "git grep 'constexpr inline' -- ':(exclude)*.sh'"
    assertFalse "git grep 'const inline' -- ':(exclude)*.sh'"
    assertFalse "git grep 'inline static' -- ':(exclude)*.sh'"

    QuietRun popd
}

testInSrcFilenameMatchRealFilename() {
    PROJECT_ROOT_DIR=$(GetProjectRootDir)
    QuietRun pushd "$PROJECT_ROOT_DIR"

    ALL_SRC_FILES=($(git ls-files -- '*.cpp' '*.hpp' '*.py' '*.sh'))
    for src_file in "${ALL_SRC_FILES[@]}"; do
        filename=$(basename "$src_file")
        extension="${filename##*.}"

        if [ "$extension" = "cpp" ] || [ "$extension" = "hpp" ]; then
            first_line=$(head -1 "$src_file")
            comment_char='//'
        else
            first_line=$(sed '3q;d' "$src_file")
            comment_char='#'
        fi

        if [[ $first_line == $comment_char* ]] && [[ $first_line == *$extension ]]; then
            assertEquals "$first_line" "$comment_char $filename"
        fi
    done

    QuietRun popd
}

source /usr/bin/shunit2
