//
// Created by melissaa on 20/04/24.
//
#include "ViewportSplitter.h"
using namespace Components;

void ViewportSplitter::moveSplitterProxy(int pos, int index) {
    this->blockSignals(true);
    this->moveSplitter(pos, index);
    this->blockSignals(false);
}
