# DPCM_Bitcrusher

![screenshot](screenshot.png)

## Download Link
https://github.com/potatoTeto/DPCM_Bitcrusher/releases

## Overview

**DPCM_Bitcrusher** is a JUCE-based **VST3 / Standalone plugin** that replicates the classic **DPCM (Differential Pulse-Code Modulation)** bitcrushing effect heard on the NES 2A03 sound chip.

This VST uses a similar algorithm to **FamiTracker** to emulate the NES DPCM channel sound characteristics. While there are some subtle audible differences, the effect should still be hardware-accurate and brings authentic NES-style bitcrushing to your DAW projects.

---

## Building from Source

### Dependencies

- [JUCE](https://juce.com/) — Required to compile the project.

### Instructions

1. Clone the repository:
    ```sh
    git clone https://github.com/potatoTeto/DPCM_Bitcrusher.git
    cd DPCM_Bitcrusher
    ```

2. Ensure **Visual Studio 2022** (or compatible version) is installed and added to your system PATH.

3. Open ``dpcm_bitcrusher.jucer`` in ``JUCE/Projucer.exe`` and then hit ``File -> Save``. This should generate an entire ``/Build/`` directory from within this project's root directory. You might need to re-map the Module directories: By default, they should all be located at ``C:\JUCE\modules\``.

4. Run the ``build-and-package.ps1`` Powershell script, passing the version tag as an argument:
    ```powershell
    ./build-and-package.ps1 v1.0.0
    ```
    Replace `v1.0.0` with your desired version string.

5. The compiled binaries will end up in packaged ``.7z`` archives from within the `publish` folder.

---

## License

This project is licensed under the GPLv3 License. See the [LICENSE](LICENSE) file for details.

---

## Contact & Maintenance

I currently have no plans to actively maintain this repository, but **Pull Requests are always welcome**! For questions or issues, please open an issue on the GitHub repository.
