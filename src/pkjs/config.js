// Clay Config: see https://github.com/pebble/clay
module.exports = [
  {
    "type": "heading",
    "defaultValue": "Settings"
  },
  {
  "type": "heading",
  "defaultValue": "Weather will not show unless a weather provider is selected and set up.",
  "size":6
  },
  {
  "type": "heading",
  "defaultValue": "Set complications to the same colour as the background to hide them.",
  "size":6
  },
  {
    "type": "section",
    "items": [
      {
        "type": "toggle",
        "messageKey": "VibeOn",
        "label": "Switch on vibrate on Bluetooth disconnect during Quiet Time",
        "defaultValue": false
      },
      {
        "type": "heading",
        "defaultValue": "Time Section",
        "size":4
      },
      {
        "type": "color",
        "messageKey": "FrameColor2",
        "defaultValue": "0x000000",
        "label": "Background Colour",
        "allowGray":true
      },
      {
        "type": "toggle",
        "messageKey": "AddZero12h",
        "label": "Add leading zero to 12h time",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "RemoveZero24h",
        "label": "Remove leading zero from 24h time",
        "defaultValue": false
      },
      {
        "type": "color",
        "messageKey": "HourColor",
        "defaultValue": "0xFFFFFF",
        "label": "Hour Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "MinColor",
        "defaultValue": "0xFFFFFF",
        "label": "Minute Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "DateColor",
        "defaultValue": "0xFFFFFF",
        "label": "Date Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text3Color",
        "defaultValue": "0xFFFFFF",
        "label": "Day of the Week Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text6Color",
        "defaultValue": "0xFFFFFF",
        "label": "Battery Bar Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text5Color",
        "defaultValue": "0xFFFFFF",
        "label": "BT & QT Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text7Color",
        "defaultValue": "0xFFFFFF",
        "label": "Weather icon Colour",
        "allowGray":true
      },
      {
        "type": "color",
        "messageKey": "Text8Color",
        "defaultValue": "0xFFFFFF",
        "label": "Temperature Colour",
        "allowGray":true
      },
      {
       "type": "color",
       "messageKey": "Text2Color",
       "defaultValue": "0xFFFFFF",
       "label": "Sunrise & Sunset Colour",
       "allowGray":true
      },
    ]
  },
  {
    "type": "section",
    "items": [
          {
            "type": "heading",
            "defaultValue": "Weather settings"
          },
          {
            "type": "toggle",
            "messageKey": "UseWeather",
            "label": "Show Weather data.",
            "defaultValue": false
          },
          {
            "type": "slider",
            "messageKey": "UpSlider",
            "defaultValue": 30,
            "label": "Weather update frequency (minutes)",
            "description": "More frequent requests will drain your phone battery more quickly",
            "min": 15,
            "max": 120,
            "step": 15
          },
          {
            "type": "toggle",
            "messageKey": "WeatherUnit",
            "label": "Temperature in Fahrenheit",
            "defaultValue": false,
          },
          {
            "type": "select",
            "messageKey": "WeatherProv",
            "defaultValue": "ds",
            "label": "Weather Provider",
            "options": [
              {
                "label": "Open-Meteo",
                "value": "ds"
              },
              {
                "label": "OpenWeatherMap",
                "value": "owm"
              }
            ]
          },
          {
             "type": "input",
             "messageKey": "Lat",
             "defaultValue": "",
             "label": "Manual Location - Latitude",
             "attributes": {
             "placeholder": "eg: 51.4962 (leave blank to use GPS)"
             }
           },
           {
              "type": "input",
              "messageKey": "Long",
              "defaultValue": "",
              "label": "Manual Location - Longitude",
              "description": "Leave both blank to use GPS location for sunrise & sunset times and weather. You can use <a href =https://www.google.com/maps>Google Maps</a> or <a href =https://www.openstreetmap.org/>OpenStreetMap</a> to find latitude & longitude.",
              "attributes": {
                "placeholder": "eg: -0.0989 (leave blank to use GPS)"
              }
            },
         {
            "type": "input",
            "messageKey": "APIKEY_User",
            "defaultValue": "",
            "label": "API Key",
            "description": "Weather data uses Open-Meteo by default which does not require an API key.  If you prefer OpenWeatherMap, you can <a href =https://home.openweathermap.org/users/sign_up/>register for a free personal API key here</a>.",
            "attributes": {
              "placeholder": "Paste your OpenWeatherMap API Key here, or leave blank for Open-Meteo"
            }
          },
        ]
          },
        {
          "type": "submit",
          "defaultValue":"SAVE"
          },
          {
          "type": "heading",
          "defaultValue": "version v2.8",
          "size":6
          },
          {
          "type": "heading",
          "defaultValue": "Made in the UK",
          "size":6
          }
        ];
