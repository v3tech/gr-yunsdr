

## How to Build GNU Radio

### PyBOMBS
PyBOMBS (Python Build Overlay Managed Bundle System) is the recommended 
method for building and installing GNU Radio. Please see 
https://github.com/gnuradio/pybombs for detailed instructions. Abbreviated 
instructions are duplicated below.

1. Install PyBOMBS:
    ```
    $ [sudo] pip install PyBOMBS
    ```
    or
    ```
    $ git clone https://github.com/gnuradio/pybombs.git
    $ cd pybombs
    $ sudo python setup.py install
    ```

2. Add PyBOMBS recipes:
    ```
    $ pybombs recipes add gr-recipes git+https://github.com/gnuradio/gr-recipes.git  
    $ pybombs recipes add gr-etcetera git+https://github.com/gnuradio/gr-etcetera.git
    ```

3. Configure an installation prefix:
    ```
    $ pybombs prefix init ~/prefix/default/
    ```

4. Install GNU Radio:
    ```
    $ pybombs install gnuradio
    ```

5. Run GNU Radio Companion from your new prefix:
    ```
    $ source ~/prefix/default/setup_env.sh
    $ gnuradio-companion
    ```
    or execute it without changing the current environment
    ```
    $ pybombs run gnuradio-companion
    ```

### Manual Source Build
Complete build instructions are detailed in the 
[GNU Radio Build Guide](https://gnuradio.org/doc/doxygen/build_guide.html). 
Abbreviated instructions are duplicated below.

1. Ensure that you have satisfied the external dependencies, see 
[GNU Radio Dependencies](https://gnuradio.org/doc/doxygen/build_guide.html).

2. Checkout the latest code:
    ```
    $ git clone https://github.com/gnuradio/gnuradio.git
    ```

3. Build with CMake:
    ```
    $ cd gnuradio
    $ mkdir build
    $ cd build
    $ cmake [OPTIONS] ../
    $ make
    $ make test
    $ sudo make install
    ```
    Useful `[OPTIONS]` include setting the install prefix 
    `-DCMAKE_INSTALL_PREFIX=<directory to install to>` and the build type 
    `-DCMAKE_BUILD_TYPE=<type>`. Currently, GNU Radio has a `"Debug"` type 
    that builds with `-g -O2` which is useful for debugging the software, 
    and a `"Release"` type that builds with `-O3`, which is the default.

## How to Build gr-yunsdr
### gr-yunsdr
1.	Source GNU Radio env:
    ```
    $ source ~/prefix/default/setup_env.sh
    ```
2.	Compile and install libyunsdr for gr-yunsdr:
    ```
    $ git clone https://github.com/v3best/libyunsdr
    $ cd libyunsdr
    $ mkdir build
    $ cd build
    $ cmake ../
    $ make && sudo make install && sudo ldconfig
    ```
3.	Compile and install gr-yunsdr to GNU Radio:
    ```
    $ git clone https://github.com/v3best/gr-yunsdr
    $ cd gr-yunsdr
    $ mkdir build
    $ cd build
    $ cmake ../ -DCMAKE_INSTALL_PREFIX=~/prefix/default/
    $ make && make install && sudo ldconfig
    ```
4.	Use gr-yunsdr block in GNU Radio Companion:
    ```
    $ source ~/prefix/default/setup_env.sh
    $ gnuradio-companion
    ``` 
<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/yunsdr_source.png" width="75%" />
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/yunsdr_sink.png" width="75%" />
</p>

### gr-osmosdr-gqrx

```
$git clone https://github.com/v3best/gr-osmosdr-gqrx
$cd gr-osmosdr-gqrx/
$git checkout yunsdr
$mkdir build
$cd build/
$cmake ../ -DCMAKE_INSTALL_PREFIX=<gnuradio install prefix>
$make
$make install
$sudo ldconfig
```
<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/osmocom_sink_1chan.png" width="75%" />
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/osmocom_sink_2chan.png" width="75%" />
</p>

### sdrangelove
```
$sudo apt install libqt5multimedia5-plugins qtmultimedia5-dev qttools5-dev qttools5-dev-tools libqt5opengl5-dev qtbase5-dev
$git clone git://git.osmocom.org/sdrangelove.git
$cd sdrangelove
$mkdir build
$cd build
$cmake ../
$make -j4
$./sdrangelove
```
<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/sdrangelove.png" width="75%" />
</p> 

### gqrx
```
$git clone https://github.com/csete/gqrx.git gqrx.git
$sudo apt install libqt5svg5-dev
$cd gqrx.git
$mkdir build
$cd build
$cmake ../
$make -j4
$./src/gqrx
```
<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/gqrx_config.png" width="75%" />
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/gqrx.png" width="75%" />
</p> 
 

### gnss-sdr
```
$git clone https://github.com/v3best/gnss-sdr
$cd gnss-sdr/
$git checkout yunsdr
$mkdir build
$cd build/
$cmake ../ 
$make
$cd ../
$mkdir temp
$cd temp
$../install/gnss-sdr --config_file=../conf/gnss-sdr_GPS_L1_yunsdr_realtime.conf

```
<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/gnss_sdr.png" width="75%" />
</p> 


### gr-dvbt

1. Build and install
    ```
    $git clone https://github.com/BogdanDIA/gr-dvbt
    $cd gr-dvbt
    $mkdir build
    $cd build/
    $cmake ../ -DCMAKE_INSTALL_PREFIX=<gnuradio install prefix>
    $make && make install && sudo ldconfig
    ```

2. Run TX

    The simplest way to run DVB-T encoding is to use the gnuradio-companion flowgraphs examples/dvbt_tx_demo.grc. It will start with a MPEG-2 TS file and will eventually generate the 10Msps baseband samples.
    Open dvbt_tx_demo.grc and run it for transmitting with YunSDR from    samples.ts. This specific flowgraph has the parameters set as: 2k OFDM, FEC code 1/2, Modulation 16-QAM, Guard Interval 1/32.

<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/dvbt_tx.png" width="75%" />
</p>

3. Run RX

    To run DVB-T decoding just run examples/dvbt_rx_demo.grc. It will take the baseband samples and turn them into MPEG-2 TS file.

<p align="center">
<img src="https://github.com/v3best/gr-yunsdr/blob/master/examples/dvbt_rx.png" width="75%" />
</p>

