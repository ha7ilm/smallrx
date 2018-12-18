# smallrx

This is an amateur radio receiver in no more than 100 lines of code.  
(If we only count code, not the comments that make it easily understandable.)

It can receive AM/NFM/SSB transmissions with an RTL-SDR.

:tv: **Presentation:**

For more insight, you can [watch the talk](https://www.youtube.com/watch?v=-QERqK1XAy0) I gave on this project at SDRA-2018.

:capital_abcd: **The block diagram of its internals:**

```                       
   IQ samples from SDR HW -> [U8->float conversion] -> [Frequency translation] ----------+
                                                                                         |
   audio output <- [float->S16 conversion] <+- [FM demod] <-+ [Decimating FIR filter]  <-+
                                            +- [AM demod] <-+
                                            +- [SSB demod] <+
```

By default, it requires a 240 ksps complex U8 input signal, and generates a 48 ksps S16 output audio signal.

:hammer: **To build:**
```bash
git clone https://github.com/ha7ilm/smallrx
cd smallrx
make
```

:hash: **Command line parameters:**
```bash
./rx <centeri_frequency_in_hz> <receiver_frequency_in_hz> <l[sb]|u[sb]|a[m]|f[m]>
```

:headphones: **To demodulate NFM at 145.325 MHz using an RTL-SDR:**
```bash
rtl_sdr -f145300000 - | ./rx 145300000 145325000 f | aplay -c1 -fS16_LE -r48000
```

:headphones: **To demodulate AM at 0.873 MHz using a HackRF:**
```bash
hackrf_transfer -s 2400000 -f 893000 -a1 -r- | csdr convert_s8_f | csdr fir_decimate_cc 10 | csdr gain_ff 30 | csdr convert_f_u8 | ./rx 893000 873000 a | mplayer -cache 1024 -quiet -rawaudio samplesize=2:channels=1:rate=48000 -demuxer rawaudio -
```

:rocket: **To test throughput:**
```bash
cat /dev/urandom | ./rx 145300000 145325000 f | csdr through > /dev/null
```

:blue_book: **Notes:**

* Some examples here need [csdr](https://github.com/simonyiszk/csdr) to be installed (also check that out if you need a fully fledged command-line DSP tool for SDR).
* HG2ECZ has translated this to Rust, which is available here: https://github.com/hg2ecz/smallrx
* For an even shorter FM demodulator (from where the idea of `smallrx` came from), see: https://github.com/simonyiszk/minidemod

----

## Contributors

- András Retzler, HA7ILM
- Zsolt Krüpl, HG2ECZ, [@hg2ecz](https://github.com/hg2ecz)
- Ádám Kiss, HA8KDA, [@kissadamfkut](https://github.com/kissadamfkut)
