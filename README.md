# wio_time_tracker

![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/InzynierDomu/wio_time_tracker/main.yml?logo=github&style=flat-square)
![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/InzynierDomu/wio_time_tracker?style=flat-square)
<a href="https://discord.gg/KmW6mHdg">![Discord](https://img.shields.io/discord/815929748882587688?logo=discord&logoColor=green&style=flat-square)</a>
![GitHub](https://img.shields.io/github/license/InzynierDomu/wio_time_tracker?style=flat-square)

## Description 

The device allows to tacking the way how spend your time, for better control and analysis.

<div align="center">
<h2>Support</h2>

<p>If any of my projects have helped you in your work, studies, or simply made your day better, you can buy me a coffee. <a href="https://buycoffee.to/inzynier-domu" target="_blank"><img src="https://buycoffee.to/img/share-button-primary.png" style="width: 195px; height: 51px" alt="Postaw mi kawę na buycoffee.to"></a></p>
</div>

### plan for new feature
- google calendar support
- pomodoro sessions
- other log formats
- easier form of synchronization
- graphic / personalization options
- other hardware platforms

## Manual
#### **Required Components**  
To operate the device, an SD card is required with two configuration files, which can be found in the `SD_example` folder.  

#### **WiFi Configuration**  
`wifi.json` The device needs a WiFi network name and password to connect to the internet and synchronize its time and date.  

#### **Timer Configuration**  
`config.json` contains a list of timers categorized into three groups.  

#### **Device Operation**  
- After powering on, categories can be switched using the top buttons.  
- Timers within a category can be navigated using the up and down arrows.  
- To start or stop a timer, press the center button on the joystick.  
- When a timer is running, the total daily value can be manually adjusted with right/left arrows.  
- The display shows two timers:  
  - **Current session time** at the top.  
  - **Total time for the day** at the bottom.  

#### **Data Storage**  
- Timer results are saved on the SD card with filenames corresponding to the date.  
- The saved format is currently Markdown (`.md`), compatible with the <a href="https://obsidian.md/">Obsidian</a>.

### **Using the Saved Files in Obsidian**  
- The `.md` files could be moved to a folder within your Obsidian vault.  
- Once in Obsidian, these files can be utilized with plugins like **Database** and **Chart** to generate visualizations.
![chart](https://github.com/InzynierDomu/wio_time_tracker/blob/main/Obsidian_example/chart_example.jpg)
- Example usage for creating charts can be found in the **Obsidian_example** folder.
## Scheme
Power supply scheme<br>
![schem](https://github.com/InzynierDomu/wio_time_tracker/blob/main/time_tracker_schem.jpg)

## IDE
The project is prepared for the Platform IO environment. A video on how to install such an environment can be watched on this [video PL](https://youtu.be/Em9NuebT2Kc).
<br><br>

Formatting is done using clang-format. The description of the tool configuration is in the [video PL](https://youtu.be/xxuaOG0WjIE).
<br><br>
