#include <pebble.h>
#include "main.h"
#include "weekday.h"
//#include "effect_layer.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/fpath.h>
#include <pebble-fctx/ffont.h>


#define TAP_COOLDOWN_MS 1000  // 1 second cooldown

//Static and initial vars
static GFont FontDayOfTheWeekShortName, FontBTQTIcons, FontTemp, FontTempFore, FontWeatherIcons, FontWeatherCondition;

#ifndef PBL_PLATFORM_APLITE
static GFont FontUV; // UV font only exists on non-Aplite
static uint32_t s_last_tap_time = 0;
#endif

FFont* time_font;
char  citistring[24];


static Window * s_window;
static Layer * s_canvas_background;
static Layer * s_canvas;
#ifndef PBL_PLATFORM_APLITE
static Layer * s_canvas_uv;
#endif
static Layer * s_canvas_weather;
static Layer * s_canvas_bt_icon;
static Layer * s_canvas_qt_icon;
Layer * time_area_layer;
Layer * time_area_layer_weather;

static int s_hours, s_minutes, s_weekday, s_day;

#ifndef PBL_PLATFORM_APLITE
static int
s_uvmax_level, s_uvnow_level;//, s_countdown;// last one changed, s_loop;
#endif


static char* weather_conditions[] = {
  "\U0000F07B", // 'unknown': 0,
  "\U0000f00e", //thunderstorm with light rain: 1
  "\U0000f02c", //thunderstorm with light rain: 2
  "\U0000f010", //thunderstorm with rain: 3
  "\U0000f02d", //thunderstorm with rain: 4
  "\U0000f01e", //thunderstorm with heavy rain: 5
  "\U0000f01e", //thunderstorm with heavy rain: 6
  "\U0000f005", //light thunderstorm: 7
  "\U0000f025", //light thunderstorm: 8
  "\U0000f01e", //thunderstorm: 9
  "\U0000f01e", //thunderstorm: 10
  "\U0000f01e", //heavy thunderstorm: 11
  "\U0000f01e", //heavy thunderstorm: 12
  "\U0000f01e", //ragged thunderstorm: 13
  "\U0000f01e", //ragged thunderstorm: 14
  "\U0000f00e", //thunderstorm with light drizzle: 15
  "\U0000f02c", //thunderstorm with light drizzle: 16
  "\U0000f00e", //thunderstorm with drizzle: 17
  "\U0000f02c", //thunderstorm with drizzle: 18
  "\U0000f01d", //thunderstorm with heavy drizzle: 19
  "\U0000f01d", //thunderstorm with heavy drizzle: 20
  "\U0000f00b", //light intensity drizzle: 21
  "\U0000f02b", //light intensity drizzle: 22
  "\U0000f01c", //drizzle: 23
  "\U0000f01c", //drizzle: 24
  "\U0000f01a", //heavy intensity drizzle: 25
  "\U0000f01a", //heavy intensity drizzle: 26
  "\U0000f00b", //light intensity drizzle rain: 27
  "\U0000f02b", //light intensity drizzle rain: 28
  "\U0000f00b", //drizzle rain: 29
  "\U0000f029", //drizzle rain: 30
  "\U0000f019", //heavy intensity drizzle rain: 31
  "\U0000f019", //heavy intensity drizzle rain: 32
  "\U0000f01a", //shower rain and drizzle: 33
  "\U0000f01a", //shower rain and drizzle: 34
  "\U0000f01a", //heavy shower rain and drizzle: 35
  "\U0000f01a", //heavy shower rain and drizzle: 36
  "\U0000f00b", //shower drizzle: 37
  "\U0000f02b", //shower drizzle: 38
  "\U0000f01a", //light rain: 39
  "\U0000f01a", //light rain: 40
  "\U0000f019", //moderate rain: 41
  "\U0000f019", //moderate rain: 42
  "\U0000f019", //heavy intensity rain: 43
  "\U0000f019 ", //heavy intensity rain: 44
  "\U0000f019", //very heavy rain: 45
  "\U0000f019", //very heavy rain: 46
  "\U0000f018", //extreme rain: 47
  "\U0000f018", //extreme rain: 48
  "\U0000f017", //freezing rain: 49
  "\U0000f017", //freezing rain: 50
  "\U0000f01a", //light intensity shower rain: 51
  "\U0000f01a", //light intensity shower rain: 52
  "\U0000f01a", //shower rain: 53
  "\U0000f01a", //shower rain: 54
  "\U0000f01a", //heavy intensity shower rain: 55
  "\U0000f01a", //heavy intensity shower rain: 56
  "\U0000f018", //ragged shower rain: 57
  "\U0000f018", //ragged shower rain: 58
  "\U0000f00a", //light snow: 59
  "\U0000f02a", //light snow: 60
  "\U0000f01b", //Snow: 61
  "\U0000f01b", //Snow: 62
  "\U0000f076", //Heavy snow: 63
  "\U0000f076", //Heavy snow: 64
  "\U0000f017", //Sleet: 65
  "\U0000f017", //Sleet: 66
  "\U0000f0b2", //Light shower sleet: 67
  "\U0000f0b4", //Light shower sleet: 68
  "\U0000f0b5", //Shower sleet: 69
  "\U0000f0b5", //Shower sleet: 70
  "\U0000f006", //Light rain and snow: 71
  "\U0000f026", //Light rain and snow: 72
  "\U0000f017", //Rain and snow: 73
  "\U0000f017", //Rain and snow: 74
  "\U0000f00a", //Light shower snow: 75
  "\U0000f02a", //Light shower snow: 76
  "\U0000f00a", //Shower snow: 77
  "\U0000f02a", //Shower snow: 78
  "\U0000f076", //Heavy shower snow: 79
  "\U0000f076", //Heavy shower snow: 80
  "\U0000f003", //mist: 81
  "\U0000f04a", //mist: 82
  "\U0000f062", //Smoke: 83
  "\U0000f062", //Smoke: 84
  "\U0000f0b6", //Haze: 85
  "\U0000f023", //Haze: 86
  "\U0000f082", //sand/ dust whirls: 87
  "\U0000f082", //sand/ dust whirls: 88
  "\U0000f014", //fog: 89
  "\U0000f014", //fog: 90
  "\U0000f082", //sand: 91
  "\U0000f082", //sand: 92
  "\U0000f082", //dust: 93
  "\U0000f082", //dust: 94
  "\U0000f0c8", //volcanic ash: 95
  "\U0000f0c8", //volcanic ash: 96
  "\U0000f011", //squalls: 97
  "\U0000f011", //squalls: 98
  "\U0000f056", //tornado: 99
  "\U0000f056", //tornado: 100
  "\U0000f00d", //clear sky: 101
  "\U0000f02e", //clear sky: 102
  "\U0000f00c", //few clouds: 11-25%: 103
  "\U0000f081", //few clouds: 11-25%: 104
  "\U0000f002", //scattered clouds: 25-50%: 105
  "\U0000f086", //scattered clouds: 25-50%: 106
  "\U0000f041", //broken clouds: 51-84%: 107
  "\U0000f041", //broken clouds: 51-84%: 108
  "\U0000f013", //overcast clouds: 85-100%: 109
  "\U0000f013", //overcast clouds: 85-100%: 110
  "\U0000f056", //tornado: 111
  "\U0000f01d", //storm-showers: 112
  "\U0000f073", //hurricane: 113
  "\U0000f076", //snowflake-cold: 114
  "\U0000f072", //hot: 115
  "\U0000f050", //windy: 116
  "\U0000f015", //hail: 117
  "\U0000f050", //strong-wind: 118
};

//////Init Configuration///
//Init Clay
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings(){
  settings.HourColorN = GColorBlack;
  settings.MinColorN = GColorBlack;
  settings.DateColorN = GColorBlack;
  settings.FrameColor2N = GColorWhite;
  settings.FrameColorN = GColorWhite;
  settings.Text1ColorN = GColorBlack;
  settings.Text2ColorN = GColorBlack;
  settings.Text3ColorN = GColorBlack;
  settings.Text4ColorN = GColorBlack;
  settings.Text5ColorN = GColorBlack;
  settings.Text6ColorN = GColorBlack;
  settings.Text7ColorN = GColorBlack;
  settings.Text8ColorN = GColorBlack;
  settings.Text9ColorN = GColorBlack;
  settings.Text10ColorN = GColorBlack;
  settings.FrameColor2 = GColorBlack;
  settings.FrameColor = GColorBlack;
  settings.Text1Color = GColorWhite;
  settings.Text2Color = GColorWhite;
  settings.Text3Color = GColorWhite;
  settings.Text4Color = GColorWhite;
  settings.Text5Color = GColorWhite;
  settings.Text6Color = GColorWhite;
  settings.Text7Color = GColorWhite;
  settings.Text8Color = GColorWhite;
  settings.Text9Color = GColorWhite;
  #ifndef PBL_PLATFORM_APLITE
    settings.UVMaxColor = GColorWhite;
    settings.UVNowColor = GColorDarkGray;
    settings.UVArcColor = GColorLightGray;
  #endif
  settings.Text10Color = GColorWhite;
  settings.HourColor = GColorWhite;
  settings.MinColor = GColorWhite;
  settings.DateColor = GColorWhite;
  settings.WeatherUnit = 0;
  settings.UseWeather = false;
  settings.UpSlider = 30;
  settings.NightTheme = false;
  settings.HealthOff = true;
  settings.VibeOn = false;
  settings.AddZero12h = false;
  settings.RemoveZero24h = false;
  //settings.UVIndexMax = 10;
  //settings.UVIndexNow = 5;
  }
int HourSunrise=700;
int HourSunset=2200;
int moonphase=0;
bool BTOn=true;
bool GPSOn=true;
bool IsNightNow=false;
bool showWeather = false;
int s_countdown = 29;
int s_loop = 0;




static GColor ColorSelect(GColor ColorDay, GColor ColorNight){
  if (settings.NightTheme && IsNightNow && GPSOn){
    return ColorNight;
  }
  else{
    return ColorDay;
  }
}


///BT Connection
static void bluetooth_callback(bool connected){
   //APP_LOG(APP_LOG_LEVEL_DEBUG, "bt 263 triggered");
  BTOn = connected;
}

static void bluetooth_callback2(bool connected2){
   //APP_LOG(APP_LOG_LEVEL_DEBUG, "bt 267 triggered");
  BTOn = connected2;
}


static void bluetooth_vibe (bool connected) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "bt 272 triggered");
  layer_set_hidden(s_canvas_bt_icon, connected);

  if(!connected && !quiet_time_is_active()) {
    // Issue a vibrating alert
    vibes_double_pulse();
  }
  else{
  if(!connected && quiet_time_is_active() && settings.VibeOn) {
      // Issue a vibrating alert
      vibes_double_pulse();
    }
  }
}

static void bluetooth_icon (bool connected2) {

  layer_set_hidden(s_canvas_bt_icon, connected2);

}


///Show sound off icon if Quiet time is active
static void quiet_time_icon () {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "qt 295 triggered");
  if(!quiet_time_is_active()) {
  layer_set_hidden(s_canvas_qt_icon,true);
  }
}


static AppTimer *s_timeout_timer;


static void  timeout_handler(void *context) {

//APP_LOG(APP_LOG_LEVEL_DEBUG, "timeout 307 triggered");
showWeather = false;
layer_set_hidden (time_area_layer, showWeather);
layer_set_hidden (s_canvas, showWeather);
layer_set_hidden (time_area_layer_weather, !showWeather);
layer_set_hidden (s_canvas_weather, !showWeather);
#ifndef PBL_PLATFORM_APLITE
    #ifdef PBL_PLATFORM_EMERY
    layer_set_hidden (s_canvas_uv, !showWeather);
    #else
    layer_set_hidden (s_canvas_uv, PBL_IF_ROUND_ELSE(!showWeather, showWeather));
    #endif
#endif
//layer_set_hidden (s_canvas_bt_icon, connected2);
bluetooth_icon(connection_service_peek_pebble_app_connection());
if(!quiet_time_is_active()) {
layer_set_hidden(s_canvas_qt_icon,true);
}
else {
layer_set_hidden(s_canvas_qt_icon,false);
}


}


/*static void time_layer_change (bool showWeather){


layer_set_hidden (time_area_layer, showWeather);
s_timeout_timer = app_timer_register(1000, timeout_handler,NULL);


}*/


static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // A tap event occured
//  showWeather = !showWeather;
//  time_layer_change (showWeather);
//APP_LOG(APP_LOG_LEVEL_DEBUG, "accel tap 345 triggered");
#ifndef PBL_PLATFORM_APLITE
  time_t seconds;
  uint16_t milliseconds;
  time_ms(&seconds, &milliseconds);
  
  uint32_t now = (uint32_t)seconds * TAP_COOLDOWN_MS + milliseconds;

  // Check cooldown
  if (s_last_tap_time != 0 && (now - s_last_tap_time < TAP_COOLDOWN_MS)) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Tap ignored");
    return;
  }
  
  s_last_tap_time = now;
  
#endif

#ifdef PBL_PLATFORM_EMERY

if(settings.UseWeather){
  showWeather = true;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Valid Tap! Show Weather");
  if(showWeather){
  layer_set_hidden (time_area_layer, showWeather);
  layer_set_hidden (s_canvas, showWeather);
  layer_set_hidden (time_area_layer_weather, !showWeather);
  layer_set_hidden (s_canvas_weather, !showWeather);
  layer_set_hidden (s_canvas_uv, !showWeather);
  layer_set_hidden (s_canvas_bt_icon, showWeather);
  layer_set_hidden (s_canvas_qt_icon, showWeather);
  }
  else {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Valid Tap, but weather off");
    layer_set_hidden (time_area_layer, !showWeather);
    layer_set_hidden (s_canvas, !showWeather);
    layer_set_hidden (time_area_layer_weather, showWeather);
    layer_set_hidden (s_canvas_weather, showWeather);
    layer_set_hidden (s_canvas_uv, showWeather);
    layer_set_hidden (s_canvas_bt_icon, !showWeather);
    layer_set_hidden (s_canvas_qt_icon, !showWeather);
    }

  s_timeout_timer = app_timer_register(10000, timeout_handler,NULL);
  //10 seconds
  }

#else

  if(settings.UseWeather){
  showWeather = true;
  if(showWeather){
  layer_set_hidden (time_area_layer, showWeather);
  layer_set_hidden (s_canvas, showWeather);
  layer_set_hidden (time_area_layer_weather, !showWeather);
  layer_set_hidden (s_canvas_weather, !showWeather);
  #ifndef PBL_PLATFORM_APLITE 
      layer_set_hidden (s_canvas_uv, PBL_IF_ROUND_ELSE(showWeather,showWeather));
  #endif
  layer_set_hidden (s_canvas_bt_icon, showWeather);
  layer_set_hidden (s_canvas_qt_icon, showWeather);
  }
  else {
    layer_set_hidden (time_area_layer, !showWeather);
    layer_set_hidden (s_canvas, !showWeather);
    layer_set_hidden (time_area_layer_weather, showWeather);
    layer_set_hidden (s_canvas_weather, showWeather);
    #ifndef PBL_PLATFORM_APLITE 
       layer_set_hidden (s_canvas_uv, PBL_IF_ROUND_ELSE(showWeather, !showWeather));
    #endif
    layer_set_hidden (s_canvas_bt_icon, !showWeather);
    layer_set_hidden (s_canvas_qt_icon, !showWeather);
    }

  s_timeout_timer = app_timer_register(10000, timeout_handler,NULL);
  //10 seconds
  }
 #endif
}

void layer_update_proc_background (Layer * back_layer, GContext * ctx){
  // Create Rects
  GRect bounds = layer_get_bounds(back_layer);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Redrawing background 404. Bounds height: %d", bounds.size.h);

  GRect TimeBand =
    PBL_IF_ROUND_ELSE(
      GRect(0, 0, bounds.size.w, bounds.size.h),
      GRect(0, 0, bounds.size.w, bounds.size.h));


      graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor2, settings.FrameColor2N));
      graphics_fill_rect(ctx, TimeBand,0,GCornersAll);


}


void update_time_area_layer(Layer *l, GContext* ctx) {

  
  // check layer bounds
  #ifdef PBL_ROUND
     GRect bounds = layer_get_unobstructed_bounds(l);
     GRect boundsobs = layer_get_bounds(l);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
     boundsobs = GRect(0, 0,boundsobs.size.w, boundsobs.size.h);
  #else
//     GRect bounds = GRect (0,0,144,80);
     GRect bounds = layer_get_unobstructed_bounds(l);
     GRect boundsobs = layer_get_bounds(l);
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
     boundsobs = GRect(0, 0,boundsobs.size.w, boundsobs.size.h);
  #endif

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Redrawing Time Area 436. Bounds height: %d", bounds.size.h);

  FContext fctx;


  fctx_init_context(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);


  #ifdef PBL_PLATFORM_EMERY
  int font_size_hour = boundsobs.size.h*0.6071428571; //102;
  int font_size_min = boundsobs.size.h*0.6071428571; //102;
  int font_size_date = 50;
  #elif defined (PBL_MICROPHONE)
  int font_size_hour = PBL_IF_ROUND_ELSE(boundsobs.size.h*0.53,boundsobs.size.h*0.6071428571); //102
  int font_size_min = PBL_IF_ROUND_ELSE(boundsobs.size.h*0.53,boundsobs.size.h*0.6071428571); //102;
  int font_size_date = PBL_IF_ROUND_ELSE(30,36);
  #else
  int font_size_hour = boundsobs.size.h*0.6071428571; //102;
  int font_size_min = boundsobs.size.h*0.6071428571; //102;
  int font_size_date = 36;
  #endif


  #ifdef PBL_COLOR
    fctx_enable_aa(true);
  #endif


  time_t temp = time(NULL);
  struct tm *time_now = localtime(&temp);


  char hourdraw[8];
//  snprintf(hourdraw, sizeof(hourdraw),"%s","23");
  if(clock_is_24h_style() && settings.RemoveZero24h){
      strftime(hourdraw, sizeof(hourdraw),"%k",time_now);
  } else if (clock_is_24h_style() && !settings.RemoveZero24h) {
      strftime(hourdraw, sizeof(hourdraw),"%H",time_now);
  } else if (settings.AddZero12h) {
    strftime(hourdraw, sizeof(hourdraw),"%I",time_now);
  } else {
    strftime(hourdraw, sizeof(hourdraw),"%l",time_now);
  }


  int minnow;
  minnow = s_minutes;
  char mindraw [8];
  snprintf(mindraw, sizeof(mindraw), PBL_IF_ROUND_ELSE("%02d",":%02d"), minnow);
  //snprintf(mindraw, sizeof(mindraw),"%s",":44");


  int daynow;
  daynow = s_day;
  char daydraw[8];
  snprintf(daydraw, sizeof(daydraw), "%02d", daynow);
  //snprintf(daydraw, sizeof(daydraw), "%s", "27");


  //draw hours
  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));


  FPoint hour_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size_hour);


  hour_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90, bounds.size.w - 1));

  if (bounds.size.h == boundsobs.size.h)
  {
  hour_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(100, -2));
  fctx_set_text_em_height(&fctx, time_font, font_size_hour);//102/4));
  }
  else
  {
  hour_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(100, 2));
  fctx_set_text_em_height(&fctx, time_font, font_size_hour *bounds.size.h/boundsobs.size.h); //102/4));
  }


//int width;
//width = fctx_string_width(&fctx,hourdraw,time_font);
//fctx_set_scale(&fctx,FPoint(width,font_size_hour),FPoint(144*16,PBL_IF_ROUND_ELSE(-font_size_hour,-font_size_hour*1.4*bounds.size.h/boundsobs.size.h)));




  fctx_set_offset(&fctx, hour_pos);


  fctx_draw_string(&fctx, hourdraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), PBL_IF_ROUND_ELSE(FTextAnchorBottom,FTextAnchorTop));
  fctx_end_fill(&fctx);


//draw minutes
  fctx_set_fill_color(&fctx, ColorSelect(settings.MinColor, settings.MinColorN));


  FPoint min_pos;
  fctx_begin_fill(&fctx);


  fctx_set_text_em_height(&fctx, time_font, font_size_min);


  min_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90, bounds.size.w - 1));
  if (bounds.size.h == boundsobs.size.h)
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(84, (boundsobs.size.h/4*3)-6 ));
     fctx_set_text_em_height(&fctx, time_font, font_size_min);
  }
  else
  {
    #ifdef PBL_PLATFORM_EMERY
      min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(84, (bounds.size.w/2)+12-4 +18));
    #else
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(84, (bounds.size.w/2)+12-4 ));
    #endif 
     fctx_set_text_em_height(&fctx, time_font, font_size_min *bounds.size.h/boundsobs.size.h);
  }//102/4*3));


//  int width_min;
//  width_min = fctx_string_width(&fctx,hourdraw,time_font);
//  fctx_set_scale(&fctx,FPoint(width_min,font_size_min),FPoint(144*16,PBL_IF_ROUND_ELSE(-font_size_hour,-font_size_hour*1.4*bounds.size.h/boundsobs.size.h)));








  fctx_set_offset(&fctx, min_pos);


  fctx_draw_string(&fctx, mindraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), PBL_IF_ROUND_ELSE(FTextAnchorTop,FTextAnchorMiddle));
  fctx_end_fill(&fctx);


//draw date
  fctx_set_fill_color(&fctx, ColorSelect(settings.DateColor, settings.DateColorN));


  FPoint date_pos;
  fctx_begin_fill(&fctx);


  fctx_set_text_em_height(&fctx, time_font, font_size_date);


  
  #ifdef PBL_PLATFORM_EMERY
  date_pos.x = INT_TO_FIXED(1);
  date_pos.y = INT_TO_FIXED(32);
  #else
  date_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(24, 0));
  date_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90-24+2+1, 6+16));
  #endif

  fctx_set_offset(&fctx, date_pos);


  fctx_draw_string(&fctx, daydraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft), PBL_IF_ROUND_ELSE(FTextAnchorMiddle, FTextAnchorTop));
  fctx_end_fill(&fctx);


  fctx_deinit_context(&fctx);
}


void update_time_area_layer_weather(Layer *l, GContext* ctx) {

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "weather Redraw Triggered 612");
    // check layer bounds
  #ifdef PBL_PLATFORM_EMERY
     GRect bounds = GRect(0,0,200,179);
     GRect boundsobs = GRect(0,0,200,179);
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
     boundsobs = GRect(0, 0,boundsobs.size.w, boundsobs.size.h);
   #elif defined (PBL_ROUND)
     GRect bounds =  GRect(90,37,90,103);
     GRect boundsobs = GRect(90,37,90,103);
     bounds = GRect(0, 0,bounds.size.w, bounds.size.h);
     boundsobs = GRect(0, 0,boundsobs.size.w, boundsobs.size.h);
  #else
//     GRect bounds = GRect (0,0,144,80);
     GRect bounds = GRect(0,0,144,119);
     GRect boundsobs = GRect(0,0,144,119);
     bounds = GRect(0,0,bounds.size.w,bounds.size.h);
     boundsobs = GRect(0, 0,boundsobs.size.w, boundsobs.size.h);
  #endif


  FContext fctx;


  fctx_init_context(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);


  #ifdef PBL_MICROPHONE
  int font_size_hour = PBL_IF_ROUND_ELSE(boundsobs.size.h*0.55,boundsobs.size.h*0.6071428571);; //102
  int font_size_min = PBL_IF_ROUND_ELSE(boundsobs.size.h*0.55,boundsobs.size.h*0.6071428571); //102;
//  int font_size_date = 30;
  #else
  int font_size_hour = boundsobs.size.h*0.6071428571; //102;
  int font_size_min = boundsobs.size.h*0.6071428571; //102;
//  int font_size_date = 30;
  #endif

  #ifdef PBL_COLOR
    fctx_enable_aa(true);
  #endif


  time_t temp = time(NULL);
  struct tm *time_now = localtime(&temp);


  char hourdraw[8];
//  snprintf(hourdraw, sizeof(hourdraw),"%s","23");
  if(clock_is_24h_style() && settings.RemoveZero24h){
      strftime(hourdraw, sizeof(hourdraw),"%k",time_now);
  } else if (clock_is_24h_style() && !settings.RemoveZero24h) {
      strftime(hourdraw, sizeof(hourdraw),"%H",time_now);
  } else if (settings.AddZero12h) {
    strftime(hourdraw, sizeof(hourdraw),"%I",time_now);
  } else {
    strftime(hourdraw, sizeof(hourdraw),"%l",time_now);
  }


  int minnow;
  minnow = s_minutes;
  char mindraw [8];
  snprintf(mindraw, sizeof(mindraw), "%02d", minnow);
  //snprintf(mindraw, sizeof(mindraw),"%s",":35");


/*  int daynow;
  daynow = s_day;
  char daydraw[8];
  snprintf(daydraw, sizeof(daydraw), "%d", daynow);
*/
  //draw hours
  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));


  FPoint hour_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size_hour);


  hour_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(138, bounds.size.w - 1) );


  if (bounds.size.h == boundsobs.size.h)
  {
  hour_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(96, -2));
  fctx_set_text_em_height(&fctx, time_font, font_size_hour);//102/4));
  }
  else
  {
  hour_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(96, 2));
  fctx_set_text_em_height(&fctx, time_font, font_size_hour *bounds.size.h/boundsobs.size.h); //102/4));
  }


//int width;
//width = fctx_string_width(&fctx,hourdraw,time_font);
//fctx_set_scale(&fctx,FPoint(width,font_size_hour),FPoint(144*16,PBL_IF_ROUND_ELSE(-font_size_hour,-font_size_hour*1.4*bounds.size.h/boundsobs.size.h)));




  fctx_set_offset(&fctx, hour_pos);


  fctx_draw_string(&fctx, hourdraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), PBL_IF_ROUND_ELSE(FTextAnchorBottom,FTextAnchorTop));
  fctx_end_fill(&fctx);


//draw minutes
  fctx_set_fill_color(&fctx, ColorSelect(settings.MinColor, settings.MinColorN));


  FPoint min_pos;
  fctx_begin_fill(&fctx);


  fctx_set_text_em_height(&fctx, time_font, font_size_min);


  min_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(138, bounds.size.w - 1));
  if (bounds.size.h == boundsobs.size.h)
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(92-5 , (boundsobs.size.h/4*3)-6 ));
     fctx_set_text_em_height(&fctx, time_font, font_size_min);
  }
  else
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(92-5 , (144/2)+12-4 ));
     fctx_set_text_em_height(&fctx, time_font, font_size_min *bounds.size.h/boundsobs.size.h);
  }//102/4*3));


//  int width_min;
//  width_min = fctx_string_width(&fctx,hourdraw,time_font);
//  fctx_set_scale(&fctx,FPoint(width_min,font_size_min),FPoint(144*16,PBL_IF_ROUND_ELSE(-font_size_hour,-font_size_hour*1.4*bounds.size.h/boundsobs.size.h)));








  fctx_set_offset(&fctx, min_pos);

  fctx_draw_string(&fctx, mindraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentRight), PBL_IF_ROUND_ELSE(FTextAnchorTop,FTextAnchorMiddle));
  fctx_end_fill(&fctx);


//draw date
/*  fctx_set_fill_color(&fctx, ColorSelect(settings.DateColor, settings.DateColorN));


  FPoint date_pos;
  fctx_begin_fill(&fctx);


  fctx_set_text_em_height(&fctx, time_font, font_size_date);


  date_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(150-74-23+9-2-2+4+1, 1));
  date_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(133-13+2+2-35, 6+16));


  fctx_set_offset(&fctx, date_pos);


  fctx_draw_string(&fctx, daydraw, time_font, GTextAlignmentLeft, FTextAnchorTop);
  fctx_end_fill(&fctx);*/


  fctx_deinit_context(&fctx);
}




static void layer_update_proc(Layer * layer, GContext * ctx){

//APP_LOG(APP_LOG_LEVEL_DEBUG, "date battery Redraw Triggered 793");
//add in weather info
#ifdef PBL_PLATFORM_EMERY
GRect DateRect = GRect(2, 6, 68, 32);
#else
GRect DateRect =
  (PBL_IF_ROUND_ELSE(
     GRect(4, 44+1, 44, 20),
     GRect(1, 4, 48, 24)));
#endif

#ifdef PBL_PLATFORM_EMERY
GRect BatteryRect = GRect(0,0,200,5);
#else
GRect BatteryRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,180,2),
      GRect(0,0,144,3)));
#endif

    //Battery
  int s_battery_level = battery_state_service_peek().charge_percent;

#ifdef PBL_PLATFORM_EMERY
  int width_rect = (s_battery_level * 200) / 100;
#elif defined (PBL_ROUND)
  int width_round = (s_battery_level * 180) / 100;
#else
  int width_rect = (s_battery_level * 144) / 100;
#endif

#ifdef PBL_PLATFORM_EMERY
GRect BatteryFillRect = GRect(0,0,width_rect,5);
#else
GRect BatteryFillRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,width_round,2),
      GRect(0,0,width_rect,3)));
#endif

  char battperc[6];
  snprintf(battperc, sizeof(battperc), "%d", s_battery_level);
  strcat(battperc, "%");


 //Date
 // Local language
 const char * sys_locale = i18n_get_system_locale();
 char datedraw[6];
 fetchwday(s_weekday, sys_locale, datedraw);
 char datenow[6];
 snprintf(datenow, sizeof(datenow), "%s", datedraw);


 //draw day of the week
 graphics_context_set_text_color(ctx, ColorSelect(settings.Text3Color, settings.Text3ColorN));
 graphics_draw_text(ctx, datenow, FontDayOfTheWeekShortName, DateRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);


 // Draw the battery bar background
 graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor2,settings.FrameColor2N));// GColorBlack);
 graphics_fill_rect(ctx, BatteryRect, 0, GCornerNone);


 // Draw the battery bar
 graphics_context_set_fill_color(ctx, ColorSelect(settings.Text6Color, settings.Text6ColorN));
 graphics_fill_rect(ctx,BatteryFillRect, 0, GCornerNone);


}


#ifndef PBL_PLATFORM_APLITE
static void layer_update_proc_uv(Layer * layer, GContext * ctx){
//APP_LOG(APP_LOG_LEVEL_DEBUG, "UV Redraw Triggered 867");
//add in uv info
int s_uvmax_level = settings.UVIndexMax;
int s_uvnow_level = settings.UVIndexNow;
int s_uvday_level = settings.UVIndexDay;
//int s_uvday_level = 14;

//APP_LOG(APP_LOG_LEVEL_DEBUG, "s_uvday_level is %d",s_uvday_level);


if(settings.UseWeather){
//int s_uvmax_level = settings.UVIndexMax;

#ifdef PBL_PLATFORM_EMERY
GRect UVDayValueRect = GRect(14 + 21,88 + 96 + 4,26,26);
#else
GRect UVDayValueRect =
  (PBL_IF_ROUND_ELSE(
    GRect(0,90,18,18),
    GRect(9,62,18,18)));
#endif
/*GRect UVIconRect =
  (PBL_IF_ROUND_ELSE(
    GRect(0,90,18,18),
    GRect(11,83,48,20)));
*/
#ifdef PBL_PLATFORM_EMERY
  int uv_max_width_rect = (s_uvmax_level * 200) / 10;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "uv_max_width_rect is %d",uv_max_width_rect);
#elif defined (PBL_ROUND)
  int uv_max_width_round = (s_uvmax_level * 180) / 10;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "uv_max_width_round is %d",uv_max_width_round);
#else
  int uv_max_width_rect = (s_uvmax_level * 144) / 10;
 // APP_LOG(APP_LOG_LEVEL_DEBUG, "uv_max_width_rect is %d",uv_max_width_rect);
#endif


#ifdef PBL_PLATFORM_EMERY
  int uv_now_width_rect = (s_uvnow_level * 200) / 10;
#elif defined (PBL_ROUND)
  int uv_now_width_round = (s_uvnow_level * 180) / 10;
#else
  int uv_now_width_rect = (s_uvnow_level * 144) / 10;
#endif


//GRect UVNowRect =
//    (PBL_IF_ROUND_ELSE(
//      GRect(uv_now_width_round-4,90,8,8),
//      GRect(uv_now_width_rect-4,163,8,8)));

//draw the uv arc
// Draw the max bar



//draw the uv arc
graphics_context_set_fill_color(ctx, ColorSelect(settings.UVArcColor,settings.UVArcColor));
#ifdef PBL_PLATFORM_EMERY
GRect arc_bounds = GRect (7 + 21 ,8+76 + 96 +4,40,40);
#else
GRect arc_bounds = GRect (4,8+52,28,28);
#endif
int32_t angle_start = DEG_TO_TRIGANGLE(180+30);
int32_t angle_end = DEG_TO_TRIGANGLE(360+180-30);
uint16_t inset_thickness = 2;
graphics_fill_radial(ctx,arc_bounds,GOvalScaleModeFitCircle,inset_thickness,angle_start,angle_end);

graphics_context_set_fill_color(ctx, ColorSelect(settings.UVMaxColor,settings.UVMaxColor));// GColorBlack);
//graphics_fill_rect(ctx, UVMaxRect, 0, GCornerNone);
#ifdef PBL_PLATFORM_EMERY
GRect arc_bounds_max = GRect (4 + 21 ,6+76 + 96+4,44,44);
#else
GRect arc_bounds_max = GRect (2,6+52,32,32);
#endif
int32_t angle_start_max = DEG_TO_TRIGANGLE(180+30);
int32_t angle_end_max = DEG_TO_TRIGANGLE((180+30)+ ((360-60)*s_uvmax_level/10));
uint16_t inset_thickness_max = 4;
graphics_fill_radial(ctx,arc_bounds_max,GOvalScaleModeFitCircle,inset_thickness_max,angle_start_max,angle_end_max);

graphics_context_set_fill_color(ctx, ColorSelect(settings.UVNowColor, settings.UVNowColor));
//graphics_fill_rect(ctx,UVNowRect, 3, GCornersAll);
#ifdef PBL_PLATFORM_EMERY
GRect arc_bounds_now = GRect (-2 + 21 ,2+76 + 96+4,52,52);
#else
GRect arc_bounds_now = GRect (-2,2+52,40,40);
#endif
    int32_t angle_start_now = DEG_TO_TRIGANGLE((180+30)+((360-60)*s_uvnow_level/10)-3);
    int32_t angle_end_now = DEG_TO_TRIGANGLE((180+30)+((360-60)*s_uvnow_level/10)+3);

uint16_t inset_thickness_now = 10;
graphics_fill_radial(ctx,arc_bounds_now,GOvalScaleModeFitCircle,inset_thickness_now,angle_start_now,angle_end_now);


char UVValueToDraw[4];
snprintf(UVValueToDraw, sizeof(UVValueToDraw), "%d",s_uvday_level);
graphics_context_set_text_color(ctx,ColorSelect(settings.UVValColor,settings.UVValColor));
if (s_uvday_level<20){
  graphics_draw_text(ctx, UVValueToDraw, FontTempFore, UVDayValueRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
}
else{
  graphics_draw_text(ctx, UVValueToDraw, FontUV, UVDayValueRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
}
//graphics_context_set_text_color(ctx, ColorSelect(settings.UVArcColor, settings.UVArcColor));
//graphics_draw_text(ctx, "F", FontUVIcon, UVIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);

}

}
#endif



static void layer_update_proc_weather(Layer * layer, GContext * ctx){

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "weather Redraw Triggered 982");
//add in weather info


#ifdef PBL_PLATFORM_APLITE

GRect SunsetRect =
     GRect(96-25-4,144+3-6,144/3,27);

GRect SunriseRect =
     GRect(96-25-4+2,144+3-21-6-3,144/3,27);

GRect SunsetIconRect =
    GRect(72+47,143,144/6,38);

GRect SunriseIconRect =
    GRect(72+47,124-3-2,144/6,38);

GRect IconNowRect = //weather condition icon
    GRect(0, 4+27+3-24, 68,54);

GRect IconForeRect = //weather condition icon
    GRect(0, 73+15+8, 68,54);

GRect TempRect =  //temperature number
    GRect(0,53,68,27);

GRect TempForeRect =  //temperature number
    GRect(0,126+15,68,27);


#elif defined (PBL_PLATFORM_EMERY)
GRect bounds = layer_get_bounds(layer);
GRect SunsetRect =
       GRect(96-25-4 +16,bounds.size.w+3-6,bounds.size.w/3,27);

GRect SunriseRect =
      GRect(96-25-4+2 +16,bounds.size.w+3-21-6-3,bounds.size.w/3,27);

GRect SunsetIconRect =
    GRect(bounds.size.w/2 +47+ 16,bounds.size.w-1,bounds.size.w/6,38);

GRect SunriseIconRect =
    GRect(bounds.size.w/2 +47 +16,bounds.size.w-20-3-2,bounds.size.w/6,38);

GRect IconNowRect = //weather condition now icon
    GRect(0, 4+27+3-24 , 68 +28,54);

GRect IconForeRect = //weather condition forecast icon
    GRect(0, 73+15+8 , 68 +28,54);

GRect TempRect =  //temperature now number
    GRect(0,53  - 6 ,68 + 28,27);

GRect TempForeRect =  //temperature high/low number
    GRect(0,126+15  - 6 + 9,68 + 28,27);
#else
GRect bounds = layer_get_bounds(layer);
GRect SunsetRect =
  (PBL_IF_ROUND_ELSE(
     GRect(0,133-8-37,54,14),
     GRect(96-25-4,bounds.size.w+3-6,bounds.size.w/3,27)));


GRect SunriseRect =
   (PBL_IF_ROUND_ELSE(
     GRect(0,133-8-37-48+2+24,54,14),
      GRect(96-25-4+2,bounds.size.w+3-21-6-3,bounds.size.w/3,27)));


GRect SunsetIconRect =
  (PBL_IF_ROUND_ELSE(
    GRect(0,150-3-36,54,24),
    GRect(bounds.size.w/2 +47,bounds.size.w-1,bounds.size.w/6,38)));


GRect SunriseIconRect =
   (PBL_IF_ROUND_ELSE(
     GRect(0,150-3-36-48+2-14,54,24),
     GRect(bounds.size.w/2 +47,bounds.size.w-20-3-2,bounds.size.w/6,38)));


GRect IconNowRect = //weather condition icon
     (PBL_IF_ROUND_ELSE(
     GRect(0-26-2+10+2+6+2, 8+91-2+2-80+3,180,32),
     GRect(0, 4+27+3-24, 68,54)));


GRect IconForeRect = //weather condition icon
     (PBL_IF_ROUND_ELSE(
     GRect(0-26-2+10+2+6+2,130-31-2+2,180,32),
     GRect(0, 73+15+8, 68,54)));


GRect TempRect =  //temperature number
    (PBL_IF_ROUND_ELSE(
       (GRect(0+20-1+10+2+6+2,24+2+109-80+3+2,90,30)),
       (GRect(0,53,68,27))));


GRect TempForeRect =  //temperature number
    (PBL_IF_ROUND_ELSE(
      (GRect(0+20-1+10+2+6+2,180-30-24-2+14+2,90,30)),
      (GRect(0,126+15,68,27))));

#endif

 //draw the sunset sunrise and moon
 char SunsetIconToShow[4];
 snprintf(SunsetIconToShow, sizeof(SunsetIconToShow),  "%s", "\U0000F052");


 char SunriseIconToShow[4];
 snprintf(SunriseIconToShow, sizeof(SunriseIconToShow),  "%s",  "\U0000F051");


 //sunsettime variable by clock setting
 char SunsetToDraw[9];
 if (clock_is_24h_style()){
   snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",settings.sunsetstring);
 }
 else {
   snprintf(SunsetToDraw, sizeof(SunsetToDraw), "%s",settings.sunsetstring12);
 }


 char SunriseToDraw[9];
 if (clock_is_24h_style()){
    snprintf(SunriseToDraw, sizeof(SunriseToDraw), "%s",settings.sunrisestring);
  }
 else {
    snprintf(SunriseToDraw, sizeof(SunriseToDraw), "%s",settings.sunrisestring12);
  }


  graphics_context_set_text_color(ctx, ColorSelect(settings.Text2Color, settings.Text2ColorN));
  graphics_draw_text(ctx, SunsetIconToShow, FontWeatherIcons, SunsetIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);
  graphics_draw_text(ctx, SunriseIconToShow, FontWeatherIcons, SunriseIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);


  graphics_context_set_text_color(ctx, ColorSelect(settings.Text2Color, settings.Text2ColorN));
  if (clock_is_24h_style()){
    graphics_draw_text(ctx, SunsetToDraw, FontTempFore, SunsetRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);
    graphics_draw_text(ctx, SunriseToDraw, FontTempFore, SunriseRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);
    }
  else {
    graphics_draw_text(ctx, SunsetToDraw, FontTempFore, SunsetRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);
    graphics_draw_text(ctx, SunriseToDraw, FontTempFore, SunriseRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentRight), NULL);
    }


 char CondToDraw[4];
 snprintf(CondToDraw, sizeof(CondToDraw), "%s",settings.iconnowstring);
 graphics_context_set_text_color(ctx,ColorSelect(settings.Text7Color,settings.Text7ColorN));
 graphics_draw_text(ctx, CondToDraw, FontWeatherCondition, IconNowRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 char CondForeToDraw[4];
 snprintf(CondForeToDraw, sizeof(CondForeToDraw), "%s",settings.iconforestring);
 graphics_context_set_text_color(ctx,ColorSelect(settings.Text7Color,settings.Text7ColorN));
 graphics_draw_text(ctx, CondForeToDraw, FontWeatherCondition, IconForeRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 char TempToDraw[8];
 snprintf(TempToDraw, sizeof(TempToDraw), "%s",settings.tempstring);
 graphics_context_set_text_color(ctx,ColorSelect(settings.Text8Color,settings.Text8ColorN));
 graphics_draw_text(ctx, TempToDraw, FontTemp, TempRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);

 char TempForeToDraw[10];
 snprintf(TempForeToDraw, sizeof(TempForeToDraw), "%s",settings.temphistring);
 graphics_context_set_text_color(ctx,ColorSelect(settings.Text8Color,settings.Text8ColorN));
 graphics_draw_text(ctx, TempForeToDraw, FontTempFore, TempForeRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);



}




static void layer_update_proc_bt(Layer * layer3, GContext * ctx3){
  //layer_set_hidden(s_canvas_bt_icon, showWeather);

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "BT Redraw Triggered 1216");

#ifdef PBL_PLATFORM_EMERY
  GRect BTIconRect = GRect(1,228/2 + 57 + 2,48,30);
#else
  GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(180-26,90-24,24,20),
      GRect(1,52+6 + 88,48,20)));
#endif

 graphics_context_set_text_color(ctx3, ColorSelect(settings.Text5Color, settings.Text5ColorN));
 graphics_draw_text(ctx3, "z", FontBTQTIcons, BTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);


}


static void layer_update_proc_qt(Layer * layer4, GContext * ctx4){

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "QT Redraw Triggered 1236");
  //layer_set_hidden(s_canvas_qt_icon, showWeather);

#ifdef PBL_PLATFORM_EMERY
  GRect QTIconRect = GRect(1,228/2 + 30 + 57,48,30);
#else
  GRect QTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(180-26,92,24,20),
      GRect(1,168-41,48,20)));

#endif
 quiet_time_icon();


 graphics_context_set_text_color(ctx4, ColorSelect(settings.Text5Color, settings.Text5ColorN));
 graphics_draw_text(ctx4, "\U0000E061", FontBTQTIcons, QTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);


}




/////////////////////////////////////////
////Init: Handle Settings and Weather////
/////////////////////////////////////////
// Read settings from persistent storage
static void prv_load_settings(){
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, & settings, sizeof(settings));
}
// Save the settings to persistent storage
static void prv_save_settings(){
  persist_write_data(SETTINGS_KEY, & settings, sizeof(settings));


}
// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator * iter, void * context){
  
  // Tuple *t = dict_read_first(iter);
  //   while(t != NULL) {
  //       APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Key: %lu", (unsigned long)t->key);
  //       t = dict_read_next(iter);
  //   }
  
  s_loop = s_loop + 1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_inbox_received_handler s_loop is %d",s_loop);


//weather
if (s_loop == 1){
  //  strcpy(settings.icon1hstring, "\U0000F04C");
  //  strcpy(settings.moonstring, "\U0000F04C");
    strcpy(settings.iconnowstring, "\U0000F03e");
}


  Tuple * pws_t = dict_find(iter, MESSAGE_KEY_UseWeather);
  if (pws_t){
    if (pws_t -> value -> int32 == 0){
      settings.UseWeather = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather switched off");
    } else {
      settings.UseWeather = true;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather switched on");
    }
  }


  Tuple * wtemp_t = dict_find(iter, MESSAGE_KEY_WeatherTemp);
  if (wtemp_t){
  snprintf(settings.tempstring, sizeof(settings.tempstring), "%s", wtemp_t -> value -> cstring);
  }


  Tuple * wforetemp_t = dict_find(iter, MESSAGE_KEY_TempFore);
  if (wforetemp_t){
    snprintf(settings.temphistring, sizeof(settings.temphistring), "%s", wforetemp_t -> value -> cstring);
  }


  Tuple * wforetemplow_t = dict_find(iter, MESSAGE_KEY_TempForeLow);
  if (wforetemplow_t){
    snprintf(settings.templowstring, sizeof(settings.templowstring), "%s", wforetemplow_t -> value -> cstring);
  }

  Tuple * frequpdate = dict_find(iter, MESSAGE_KEY_UpSlider);
  if (frequpdate){
    settings.UpSlider = (int) frequpdate -> value -> int32;
    //Restart the counter
    s_countdown = settings.UpSlider;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "tuple countdown is %d, upslsider is %d",s_countdown,settings.UpSlider);
  }

  #ifndef PBL_PLATFORM_APLITE 
      Tuple * uvmax_tuple = dict_find(iter, MESSAGE_KEY_UVIndexMax);
      if (uvmax_tuple){
        settings.UVIndexMax = (int) uvmax_tuple -> value -> int32;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "UVIndexMax is %d",settings.UVIndexMax);
      }

      Tuple * uvday_tuple = dict_find(iter, MESSAGE_KEY_UVIndexDay);
      if (uvday_tuple){
        settings.UVIndexDay = (int) uvday_tuple -> value -> int32;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "UVIndexDay is %d",settings.UVIndexDay);
      }

      Tuple * uvnow_tuple = dict_find(iter, MESSAGE_KEY_UVIndexNow);
      if (uvnow_tuple){
        settings.UVIndexNow = (int) uvnow_tuple -> value -> int32;
        s_uvnow_level = settings.UVIndexNow;
      }
  #endif


  Tuple * iconnow_tuple = dict_find(iter, MESSAGE_KEY_IconNow);
    if (iconnow_tuple){
      snprintf(settings.iconnowstring,sizeof(settings.iconnowstring),"%s",weather_conditions[(int)iconnow_tuple->value->int32]);
  }


  Tuple * iconfore_tuple = dict_find(iter, MESSAGE_KEY_IconFore);
  if (iconfore_tuple){
    snprintf(settings.iconforestring,sizeof(settings.iconforestring),"%s",weather_conditions[(int)iconfore_tuple->value->int32]);
  }


  Tuple * sunrise_t = dict_find(iter, MESSAGE_KEY_HourSunrise);
  if (sunrise_t){
    HourSunrise = (int) sunrise_t -> value -> int32;
  }
  Tuple * sunset_t = dict_find(iter, MESSAGE_KEY_HourSunset);
  if (sunset_t){
    HourSunset = (int) sunset_t -> value -> int32;
  }


  Tuple * sunset_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY);
  if (sunset_dt){
     snprintf(settings.sunsetstring, sizeof(settings.sunsetstring), "%s", sunset_dt -> value -> cstring);
  }
   Tuple * sunrise_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY);
  if (sunrise_dt){
     snprintf(settings.sunrisestring, sizeof(settings.sunrisestring), "%s", sunrise_dt -> value -> cstring);
  }


  //12hr version of sunset & sunrise
  Tuple * sunset12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNSET_KEY_12H);
  if (sunset12_dt){
     snprintf(settings.sunsetstring12, sizeof(settings.sunsetstring12), "%s", sunset12_dt -> value -> cstring);
  }
  Tuple * sunrise12_dt = dict_find(iter, MESSAGE_KEY_WEATHER_SUNRISE_KEY_12H);
  if (sunrise12_dt){
     snprintf(settings.sunrisestring12, sizeof(settings.sunrisestring12), "%s", sunrise12_dt -> value -> cstring);
  }




  //  Colours


  Tuple * sd1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor2);
  if (sd1_color_t){
    settings.FrameColor2 = GColorFromHEX(sd1_color_t-> value -> int32);
  }
  Tuple * nsd1_color_t = dict_find(iter, MESSAGE_KEY_FrameColor2N);
  if (nsd1_color_t){
    settings.FrameColor2N = GColorFromHEX(nsd1_color_t-> value -> int32);
  }
   Tuple * sd2_color_t = dict_find(iter, MESSAGE_KEY_FrameColor);
  if (sd2_color_t){
    settings.FrameColor = GColorFromHEX(sd2_color_t-> value -> int32);
  }
  Tuple * nsd2_color_t = dict_find(iter, MESSAGE_KEY_FrameColorN);
  if (nsd2_color_t){
    settings.FrameColorN = GColorFromHEX(nsd2_color_t-> value -> int32);
  }
  ////////////
  Tuple * tx1_color_t = dict_find(iter, MESSAGE_KEY_Text1Color);
  if (tx1_color_t){
    settings.Text1Color = GColorFromHEX(tx1_color_t-> value -> int32);
  }
  Tuple * ntx1_color_t = dict_find(iter, MESSAGE_KEY_Text1ColorN);
  if (ntx1_color_t){
    settings.Text1ColorN = GColorFromHEX(ntx1_color_t-> value -> int32);
  }
  ///////////////////////////////
  Tuple * hr_color_t = dict_find(iter, MESSAGE_KEY_HourColor);
  if (hr_color_t){
    settings.HourColor = GColorFromHEX(hr_color_t-> value -> int32);
  }
  Tuple * nthr_color_t = dict_find(iter, MESSAGE_KEY_HourColorN);
  if (nthr_color_t){
    settings.HourColorN = GColorFromHEX(nthr_color_t-> value -> int32);
  }
  Tuple * min_color_t = dict_find(iter, MESSAGE_KEY_MinColor);
  if (min_color_t){
    settings.MinColor = GColorFromHEX(min_color_t-> value -> int32);
  }
  Tuple * ntmin_color_t = dict_find(iter, MESSAGE_KEY_MinColorN);
  if (ntmin_color_t){
    settings.MinColorN = GColorFromHEX(ntmin_color_t-> value -> int32);
  }
  Tuple * date_color_t = dict_find(iter, MESSAGE_KEY_DateColor);
  if (date_color_t){
    settings.DateColor = GColorFromHEX(date_color_t-> value -> int32);
  }
  Tuple * ntdate_color_t = dict_find(iter, MESSAGE_KEY_DateColorN);
  if (ntdate_color_t){
    settings.DateColorN = GColorFromHEX(ntdate_color_t-> value -> int32);
  }
  ///////////////////////////////
  Tuple * tx2_color_t = dict_find(iter, MESSAGE_KEY_Text2Color);
  if (tx2_color_t){
    settings.Text2Color = GColorFromHEX(tx2_color_t-> value -> int32);
  }
  Tuple * ntx2_color_t = dict_find(iter, MESSAGE_KEY_Text2ColorN);
  if (ntx2_color_t){
    settings.Text2ColorN = GColorFromHEX(ntx2_color_t-> value -> int32);
  }
   Tuple * tx3_color_t = dict_find(iter, MESSAGE_KEY_Text3Color);
  if (tx3_color_t){
    settings.Text3Color = GColorFromHEX(tx3_color_t-> value -> int32);
  }
  Tuple * ntx3_color_t = dict_find(iter, MESSAGE_KEY_Text3ColorN);
  if (ntx3_color_t){
    settings.Text3ColorN = GColorFromHEX(ntx3_color_t-> value -> int32);
  }
  Tuple * tx4_color_t = dict_find(iter,MESSAGE_KEY_Text4Color);
  if (tx4_color_t){
    settings.Text4Color = GColorFromHEX(tx4_color_t-> value -> int32);
    }
  Tuple * ntx4_color_t = dict_find(iter, MESSAGE_KEY_Text4ColorN);
  if(ntx4_color_t){
    settings.Text4ColorN = GColorFromHEX(ntx4_color_t-> value -> int32);
  }
  Tuple * tx5_color_t = dict_find(iter,MESSAGE_KEY_Text5Color);
  if (tx5_color_t){
    settings.Text5Color = GColorFromHEX(tx5_color_t-> value -> int32);
    }
  Tuple * ntx5_color_t = dict_find(iter, MESSAGE_KEY_Text5ColorN);
  if(ntx5_color_t){
    settings.Text5ColorN = GColorFromHEX(ntx5_color_t-> value -> int32);
  }
   Tuple * tx6_color_t = dict_find(iter,MESSAGE_KEY_Text6Color);
  if (tx6_color_t){
    settings.Text6Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }
  Tuple * ntx6_color_t = dict_find(iter, MESSAGE_KEY_Text6ColorN);
  if(ntx6_color_t){
    settings.Text6ColorN = GColorFromHEX(ntx6_color_t-> value -> int32);
  }
  if (tx6_color_t){
    settings.Text6Color = GColorFromHEX(tx6_color_t-> value -> int32);
    }


  Tuple * tx7_color_t = dict_find(iter,MESSAGE_KEY_Text7Color);
 if (tx7_color_t){
   settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
   }
 Tuple * ntx7_color_t = dict_find(iter, MESSAGE_KEY_Text7ColorN);
 if(ntx7_color_t){
   settings.Text7ColorN = GColorFromHEX(ntx7_color_t-> value -> int32);
 }
 if (tx7_color_t){
   settings.Text7Color = GColorFromHEX(tx7_color_t-> value -> int32);
   }


 Tuple * tx8_color_t = dict_find(iter,MESSAGE_KEY_Text8Color);
if (tx8_color_t){
  settings.Text8Color = GColorFromHEX(tx8_color_t-> value -> int32);
  }
Tuple * ntx8_color_t = dict_find(iter, MESSAGE_KEY_Text8ColorN);
if(ntx8_color_t){
  settings.Text8ColorN = GColorFromHEX(ntx8_color_t-> value -> int32);
}
if (tx8_color_t){
  settings.Text8Color = GColorFromHEX(tx8_color_t-> value -> int32);
  }


Tuple * tx9_color_t = dict_find(iter,MESSAGE_KEY_Text9Color);
if (tx9_color_t){
 settings.Text9Color = GColorFromHEX(tx9_color_t-> value -> int32);
 }
Tuple * ntx9_color_t = dict_find(iter, MESSAGE_KEY_Text9ColorN);
if(ntx9_color_t){
 settings.Text9ColorN = GColorFromHEX(ntx9_color_t-> value -> int32);
}
if (tx9_color_t){
 settings.Text9Color = GColorFromHEX(tx9_color_t-> value -> int32);
 }


Tuple * tx10_color_t = dict_find(iter,MESSAGE_KEY_Text10Color);
if (tx10_color_t){
settings.Text10Color = GColorFromHEX(tx10_color_t-> value -> int32);
}
Tuple * ntx10_color_t = dict_find(iter, MESSAGE_KEY_Text10ColorN);
if(ntx10_color_t){
settings.Text10ColorN = GColorFromHEX(ntx10_color_t-> value -> int32);
}
if (tx10_color_t){
settings.Text10Color = GColorFromHEX(tx10_color_t-> value -> int32);
}

#ifndef PBL_PLATFORM_APLITE
    Tuple * uvarccol_t = dict_find(iter,MESSAGE_KEY_UVArcColor);
    if (uvarccol_t){
    settings.UVArcColor = GColorFromHEX(uvarccol_t-> value -> int32);
    }

    Tuple * uvmaxcol_t = dict_find(iter,MESSAGE_KEY_UVMaxColor);
    if (uvmaxcol_t){
    settings.UVMaxColor = GColorFromHEX(uvmaxcol_t-> value -> int32);
    }

    Tuple * uvvalcol_t = dict_find(iter,MESSAGE_KEY_UVValColor);
    if (uvvalcol_t){
    settings.UVValColor = GColorFromHEX(uvvalcol_t-> value -> int32);
    }

    Tuple * uvnowcol_t = dict_find(iter,MESSAGE_KEY_UVNowColor);
    if (uvnowcol_t){
    settings.UVNowColor = GColorFromHEX(uvnowcol_t-> value -> int32);
    }
#endif


////Weather and gps
  Tuple * weather_t = dict_find(iter, MESSAGE_KEY_Weathertime);
  if (weather_t){
    settings.Weathertimecapture = (int) weather_t -> value -> int32;
  //   snprintf(settings.weathertimecapture, sizeof(settings.weathertimecapture), "%s", weather_t -> value -> cstring);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather data captured at time %d",settings.Weathertimecapture);
  }


  Tuple * neigh_t = dict_find(iter, MESSAGE_KEY_NameLocation);
  if (neigh_t){
    snprintf(citistring, sizeof(citistring), "%s", neigh_t -> value -> cstring);
  }
  //Control of data gathered for http
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Location Timezone is %s", citistring);
  if (strcmp(citistring, "") == 0 || strcmp(citistring,"NotSet") == 0 ){
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Missing info at loop %d, GPS false, citistring is %s, 1 is %d, 2 is %d", s_loop, citistring, strcmp(citistring, ""),strcmp(citistring,"NotSet"));
    GPSOn = false;
  } else{
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "GPS working loop %d, GPS on, citistring is %s, 1 is %d, 2 is %d", s_loop, citistring, strcmp(citistring, ""),strcmp(citistring,"NotSet"));
    GPSOn = true;
  }


  Tuple * disntheme_t = dict_find(iter, MESSAGE_KEY_NightTheme);
  if (disntheme_t){
    if (disntheme_t -> value -> int32 == 0){
      settings.NightTheme = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "NTHeme off");
    } else {
    settings.NightTheme = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "NTHeme on");
      }
    }


  Tuple * vibe_t = dict_find(iter, MESSAGE_KEY_VibeOn);
  if (vibe_t){
    if (vibe_t -> value -> int32 == 0){
      settings.VibeOn = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibe off");
    } else {
      settings.VibeOn = true;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibe on");
    }
  }


  Tuple * addzero12_t = dict_find(iter, MESSAGE_KEY_AddZero12h);
  if (addzero12_t){
    if (addzero12_t -> value -> int32 == 0){
      settings.AddZero12h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h off");
    } else {
    settings.AddZero12h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Add Zero 12h on");
      }
    }


  Tuple * remzero24_t = dict_find(iter, MESSAGE_KEY_RemoveZero24h);
  if (remzero24_t){
    if (remzero24_t -> value -> int32 == 0){
      settings.RemoveZero24h = false;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
    } else {
    settings.RemoveZero24h = true;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Remove Zero 24h off");
      }
    }


//  layer_mark_dirty(s_canvas_weather_section);
  layer_mark_dirty(time_area_layer);
  layer_mark_dirty(s_canvas);
  layer_mark_dirty(time_area_layer_weather);
  layer_mark_dirty(s_canvas_weather);
 #ifndef PBL_PLATFORM_APLITE
  layer_mark_dirty(s_canvas_uv);
 #endif
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);


  prv_save_settings();
}


//Load main layer
static void window_load(Window * window){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "window load triggered 1663");
  
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds4 = layer_get_bounds(window_layer);


  s_canvas_background = layer_create(bounds4);
    layer_set_update_proc(s_canvas_background, layer_update_proc_background);
    layer_add_child(window_layer, s_canvas_background);


   


    time_area_layer = layer_create(bounds4);
       layer_add_child(window_layer, time_area_layer);
       layer_set_update_proc(time_area_layer, update_time_area_layer);

    s_canvas_bt_icon = layer_create(bounds4);
      //layer_set_hidden(s_canvas_bt_icon, showWeather);
      layer_set_update_proc (s_canvas_bt_icon, layer_update_proc_bt);
      layer_add_child(window_layer, s_canvas_bt_icon);


    s_canvas_qt_icon = layer_create(bounds4);
      if(!quiet_time_is_active()) {
      layer_set_hidden(s_canvas_qt_icon,true);
      } else {
      layer_set_hidden(s_canvas_qt_icon,false);
      }
      layer_set_update_proc (s_canvas_qt_icon, layer_update_proc_qt);
      layer_add_child(window_layer, s_canvas_qt_icon);


     time_area_layer_weather = layer_create(bounds4);
        layer_set_hidden (time_area_layer_weather, !showWeather);
        layer_add_child(window_layer, time_area_layer_weather);
        layer_set_update_proc(time_area_layer_weather, update_time_area_layer_weather);


     s_canvas_weather = layer_create(bounds4);
        layer_set_hidden (s_canvas_weather, !showWeather);
        layer_set_update_proc(s_canvas_weather, layer_update_proc_weather);
        layer_add_child(window_layer, s_canvas_weather);

    #ifndef PBL_PLATFORM_APLITE
    s_canvas_uv = layer_create(bounds4);
       #ifdef PBL_PLATFORM_EMERY
        layer_set_hidden (s_canvas_uv, !showWeather);
       #else
        layer_set_hidden (s_canvas_uv, PBL_IF_ROUND_ELSE(!showWeather, showWeather));
       #endif
       layer_set_update_proc(s_canvas_uv, layer_update_proc_uv);
       layer_add_child(window_layer, s_canvas_uv);
    #endif

  s_canvas = layer_create(bounds4);
     layer_set_update_proc(s_canvas, layer_update_proc);
     layer_add_child(window_layer, s_canvas);
  }




static void window_unload(Window * window){

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "window unload triggered 1728");

  layer_destroy (s_canvas_background);
  layer_destroy(time_area_layer);
  layer_destroy(time_area_layer_weather);
  layer_destroy(s_canvas_bt_icon);
  layer_destroy(s_canvas);
  layer_destroy(s_canvas_weather);
  #ifndef PBL_PLATFORM_APLITE
  layer_destroy(s_canvas_uv);
  #endif
  layer_destroy(s_canvas_qt_icon);
  window_destroy(s_window);


  ffont_destroy(time_font);
  fonts_unload_custom_font(FontDayOfTheWeekShortName);
  fonts_unload_custom_font(FontBTQTIcons);

  #ifndef PBL_PLATFORM_APLITE
   fonts_unload_custom_font(FontTemp);
   fonts_unload_custom_font(FontTempFore);
   fonts_unload_custom_font(FontUV);
  #endif
//  fonts_unload_custom_font(FontUVIcon);
  fonts_unload_custom_font(FontWeatherIcons);
  fonts_unload_custom_font(FontWeatherCondition);


}


void main_window_push(){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "window push triggered 1756");
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}


void main_window_update(int hours, int minutes, int weekday, int day){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "main window update triggered 1767");
  s_hours = hours;
  s_minutes = minutes;
  s_day = day;
  s_weekday = weekday;


  layer_mark_dirty(time_area_layer);
  layer_mark_dirty(time_area_layer_weather);
}


static void tick_handler(struct tm * time_now, TimeUnits changed){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %02d:%02d:%02d | Units: %d ", 
          time_now->tm_hour, time_now->tm_min, time_now->tm_sec, (int)changed);


if (changed & MINUTE_UNIT){
  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday);
  //update_time();
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler s_countdown %d", s_countdown);
  //s_loop = 0;
  if (s_countdown == 0 && (changed & MINUTE_UNIT)){
    //Reset
    s_countdown = settings.UpSlider;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler 2 s_countdown %d", s_countdown);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler 2 upslider %d", settings.UpSlider);
  } else if (changed & MINUTE_UNIT){
    s_countdown = s_countdown - 1;
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick handler 3 s_countdown %d", s_countdown);
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick hander 4 Countdown to update %d loop is %d, GPS is %d", s_countdown, s_loop, GPSOn);




  // Evaluate if is day or night
  int nowthehouris = s_hours * 100 + s_minutes;
  if (HourSunrise <= nowthehouris && nowthehouris <= HourSunset){
    IsNightNow = false;
  } else{
    IsNightNow = true;
  }
  // Extra catch on sunrise and sunset
  if (nowthehouris == HourSunrise || nowthehouris == HourSunset){
    s_countdown = 1;
  }
  if (GPSOn && settings.NightTheme){
    //Extra catch around 1159 to gather information of today
    if (nowthehouris == 1159 && s_countdown > 5){
      s_countdown = 1;
    };
    // Change Colors for night/day
    layer_mark_dirty(s_canvas_background);
    layer_mark_dirty(time_area_layer);
    layer_mark_dirty(s_canvas);
    layer_mark_dirty(time_area_layer_weather);
    layer_mark_dirty(s_canvas_weather);
   #ifndef PBL_PLATFORM_APLITE
    layer_mark_dirty(s_canvas_uv);
   #endif
    layer_mark_dirty(s_canvas_bt_icon);
    layer_mark_dirty(s_canvas_qt_icon);


  }
  // Get weather update every requested minutes and extra request 5 minutes earlier
  if (s_countdown == 0 || s_countdown == 5){
      APP_LOG(APP_LOG_LEVEL_DEBUG, "countdown is 0, updated weather at %d", time_now -> tm_min);
      s_loop = 0;
      // Begin dictionary
      DictionaryIterator * iter;
      app_message_outbox_begin( & iter);
      // Add a key-value pair
      dict_write_uint8(iter, 0, 0);
      // Send the message!
      app_message_outbox_send();
  }


//If GPS was off request weather every 15 minutes
  if (!GPSOn){
      if (settings.UpSlider > 15){
        if (s_countdown % 15 == 0){
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Attempt to request GPS on %d", time_now -> tm_min);
          s_loop = 0;
          // Begin dictionary
          DictionaryIterator * iter;
          app_message_outbox_begin( & iter);
          // Add a key-value pair
          dict_write_uint8(iter, 0, 0);
          // Send the message!
          app_message_outbox_send();
        }
      }
    }


  //onreconnection(BTOn, connection_service_peek_pebble_app_connection());
  if (!BTOn && connection_service_peek_pebble_app_connection()){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BT reconnected, requesting weather at %d", time_now -> tm_min);
    s_loop = 0;
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "s_loop 1870 = 0");
    // Begin dictionary
    DictionaryIterator * iter;
    app_message_outbox_begin( & iter);
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
    // Send the message!
    app_message_outbox_send();
  }


  bluetooth_callback(connection_service_peek_pebble_app_connection());
  bluetooth_callback2(connection_service_peek_pebble_app_connection());


  }
}


static void init(){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "init triggered 1883");

  prv_load_settings();


  // Listen for AppMessages


  s_countdown = settings.UpSlider;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "init s_countdown is %d, upslider is %d", s_countdown,settings.UpSlider);

  //Clean vars
  strcpy(citistring, "NotSet");
  strcpy(settings.iconnowstring, "\U0000F03D");


  s_loop = 0;


  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  s_hours=t->tm_hour;
  s_minutes=t->tm_min;
  s_day=t->tm_mday;
  s_weekday=t->tm_wday;
  //Register and open
  app_message_register_inbox_received(prv_inbox_received_handler);
#ifdef PBL_MICROPHONE
  app_message_open(1024,1024);
#else
  app_message_open(512,512);
#endif


// Load Fonts
   // allocate fonts
  time_font =  ffont_create_from_resource(RESOURCE_ID_FONT_LIZ);



#ifdef PBL_PLATFORM_EMERY
  FontDayOfTheWeekShortName = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LIZ_26));
  FontBTQTIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_22));
  FontTemp = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LIZ_37));
  FontTempFore = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LIZ_25));
//sunset sunrise icon font
  FontWeatherIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_22));
//weather icons
  FontWeatherCondition = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_WEATHERICONS_35));
  FontUV = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_LIZ_20));
  //FontUVIcon = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_DRIPICONS_18, RESOURCE_ID_FONT_DRIPICONS_18)));
#else
  FontDayOfTheWeekShortName = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_12, RESOURCE_ID_FONT_LIZ_18)));
  FontBTQTIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  //#ifdef PBL_PLATFORM_APLITE
  //  FontTemp = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  //  FontTempFore = fonts_get_system_font(FONT_KEY_GOTHIC_18);
  //#else
    FontTemp = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_27, RESOURCE_ID_FONT_LIZ_27)));
    FontTempFore = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_18, RESOURCE_ID_FONT_LIZ_18)));
  //#endif
//sunset sunrise icon font
  FontWeatherIcons = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_16,RESOURCE_ID_FONT_WEATHERICONS_16)));
//weather icons
  FontWeatherCondition = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_30,RESOURCE_ID_FONT_WEATHERICONS_30)));
  #ifndef PBL_PLATFORM_APLITE
      FontUV = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_14, RESOURCE_ID_FONT_LIZ_14)));
  #endif
  //FontUVIcon = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_DRIPICONS_18, RESOURCE_ID_FONT_DRIPICONS_18)));
#endif
  main_window_push();
  // Register with Event Services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);


  connection_service_subscribe((ConnectionHandlers){
    .pebble_app_connection_handler = bluetooth_vibe
  });
  bluetooth_vibe(connection_service_peek_pebble_app_connection());
  accel_tap_service_subscribe(accel_tap_handler);
}


static void deinit(){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit triggered 1958");
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
//  health_service_events_unsubscribe();
  accel_tap_service_unsubscribe();
}
int main(){
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "main triggered 1967");
  init();
  app_event_loop();
  deinit();
}
