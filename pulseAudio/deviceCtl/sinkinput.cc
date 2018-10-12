
#include "sinkinput.h"
#include "audiocore.h"
#include "sink.h"

SinkInput::SinkInput() {
}

void SinkInput::moveSinkInput(const char *defName) {

  pa_operation* o;
  if (!(o = pa_context_move_sink_input_by_name(AudioCore::getInstance()->context, index, defName, NULL, NULL))) {
    log("pa_context_move_sink_input_by_index() failed");
    return;
  }

  pa_operation_unref(o);
}

