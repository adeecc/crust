#include <common/sourceloc.hpp>

using namespace Crust;

void SourceLocation::init() {
    mCurrentColumn = 1;
    mCurrentLine = 1;
}

void SourceLocation::advance(bool nextLine) {
    if (nextLine) {
        ++mCurrentLine;
        mCurrentColumn = 1;
    } else {
        ++mCurrentColumn;
    }
}