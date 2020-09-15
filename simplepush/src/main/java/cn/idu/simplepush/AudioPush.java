package cn.idu.simplepush;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

class AudioPush extends BasePush {
    private AudioInfo audioInfo;

    private AudioRecord audioRecord;
    private boolean isPushing = false;
    private int minBufferSize;
    private NativePlayer player;
    private byte[] temp;

    public AudioPush(AudioInfo audioInfo, NativePlayer player) {
        this.audioInfo = audioInfo;
        this.player = player;

        int channelConfig = audioInfo.getChannel() == 1 ? AudioFormat.CHANNEL_IN_MONO : AudioFormat.CHANNEL_IN_STEREO;

        minBufferSize = AudioRecord.getMinBufferSize(audioInfo.getSampleRateInHz(), channelConfig,
                AudioFormat.ENCODING_PCM_16BIT);
        temp = new byte[minBufferSize];
        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, audioInfo.getSampleRateInHz(),
                channelConfig, AudioFormat.ENCODING_PCM_16BIT, minBufferSize);

    }

    @Override
    public void startPush() {
        if (!isPushing) {
            isPushing = true;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    audioRecord.startRecording();
                    while (isPushing) {
                        int len;
                        if ((len = audioRecord.read(temp, 0, minBufferSize)) > 0) {
                            player.sendAudioPacket(temp, len);
                        }
                    }
                }
            }).start();
        }
    }

    @Override
    public void stopPush() {
        if (isPushing) {
            isPushing = false;
            audioRecord.stop();
        }
    }

    @Override
    public void release() {
        if (audioRecord != null) {
            audioRecord.release();
            audioRecord = null;
        }
    }

}
