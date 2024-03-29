//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "libmedia_playback.h"

#include <android-base/logging.h>
#include <media/AudioTrack.h>
#include <media/stagefright/MediaSource.h>

namespace android {

MediaBuffer* LibmediaPlayback::sine_data_buffer = NULL;

void LibmediaPlayback::Init() {
  int kSampleRateHz = 8000;
  int num_channels = 1;
  source = new SineSource(kSampleRateHz, num_channels);
  source->start(NULL); // Initialize with no parameters.

  // Read data from source and store it in a buffer.
  MediaSource::ReadOptions options;
  source->read(&sine_data_buffer, &options);
}

size_t LibmediaPlayback::FillBuffer(void* data, size_t size) {
  CHECK(data != NULL);
  size = (size < sine_data_buffer->size()) ? size : sine_data_buffer->size();
  memcpy(data, sine_data_buffer->data(), size);
  return size;
}

void LibmediaPlayback::AudioCallback(int event, void* user, void* info) {
  switch (event) {
    case AudioTrack::EVENT_MORE_DATA: {
      AudioTrack::Buffer* buffer = static_cast<AudioTrack::Buffer*>(info);
      buffer->size = FillBuffer(buffer->raw, buffer->size);
      break;
    }
    case AudioTrack::EVENT_STREAM_END: {
      break;
    }
    default: {
      fprintf(stderr, "Unexpected audio callback event\n");
      break;
    }
  }
}

status_t LibmediaPlayback::Play() {
  int kSampleRateHz = 8000;
  audio_stream_type_t stream_type = AUDIO_STREAM_MUSIC;
  audio_format_t audio_format = AUDIO_FORMAT_PCM_16_BIT;
  audio_channel_mask_t audio_mask = AUDIO_CHANNEL_OUT_ALL;
  size_t frame_count = 0; // Use default value for frame count.
  audio_output_flags_t audio_output_flags = AUDIO_OUTPUT_FLAG_NONE;

  AudioTrack* track = new AudioTrack(
      stream_type, kSampleRateHz, audio_format, audio_mask, frame_count,
      audio_output_flags, LibmediaPlayback::AudioCallback);
  status_t status = track->initCheck();
  if (status != OK) {
    LOG(ERROR) << "Audio track initialization failed.";
    return status;
  }

  float volume = 1;
  track->setVolume(volume);
  status = track->start();
  if (status != OK) {
    LOG(ERROR) << "Audio track failed to start.";
    return status;
  }

  // Change volumes.
  // The volume should decrease steadily and then increase steadily again.
  for (float volume = 1; volume >= 0; volume -= 0.1) {
    track->setVolume(volume);
    sleep(1);
  }
  for (float volume = 0; volume <= 1; volume += 0.1) {
    track->setVolume(volume);
    sleep(1);
  }

  sleep(10);
  track->stop();
  // Free memory.
  sine_data_buffer->release();
  return status;
}

}  // namespace android
