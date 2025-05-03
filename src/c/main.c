#include <pebble.h>
#include "main.h"
#include "weekday.h"
//#include "effect_layer.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/fpath.h>
#include <pebble-fctx/ffont.h>


//Static and initial vars
static GFont
  FontDayOfTheWeekShortName, FontBTQTIcons, FontTemp, FontTempFore, FontWeatherIcons, FontWeatherCondition;// FontDate, FontSunset, FontMoonPhase,FontWindDirection;


FFont* time_font;


char  citistring[24];


static Window * s_window;
static Layer * s_canvas_background;
static Layer * s_canvas;
static Layer * s_canvas_weather;
static Layer * s_canvas_bt_icon;
static Layer * s_canvas_qt_icon;
Layer * time_area_layer;
Layer * time_area_layer_weather;


static int s_hours, s_minutes, s_weekday, s_day;// last one changed, s_loop;


static char* weather_conditions[] = {
  "\U0000F07B", // 'unknown': 0,
  "\U0000F00D", // 'clear': 1,
  "\U0000F00C", // 'fewclouds': 2,
  "\U0000F041", // 'scattered clouds': 3,
  "\U0000F013", // 'brokenclouds': 4,
  "\U0000F01A", // 'shower rain': 5,
  "\U0000F019", // 'rain': 6,
  "\U0000F076", // 'snow': 7,
  "\U0000F01E", // 'tstorms': 8,
  "\U0000F021", // 'mist': 9,
  "\U0000F02E", // 'nt_clear': 10,
  "\U0000F081", // 'nt_few clouds': 11,
  "\U0000F086", // 'nt_scattered clouds': 12,
  "\U0000F013", // 'nt_broken clouds' : 13
  "\U0000F029", // 'nt_shower rain': 14,
  "\U0000F028", // 'nt_rain': 15,
  "\U0000F076", // 'nt_snow': 16,
  "\U0000F016", // 'nt_tstorms': 17,
  "\U0000F021", // 'nt_mist': 18,
  "\U0000F014", // 'fog': 19,
  "\U0000F01C", // 'drizzle': 20,
  "\U0000F018", // 'heavy driving rain': 21,
  "\U0000F0B5", // 'sleet': 22,
  "\U0000F017", // 'heavy sleet': 23,
  "\U0000F01B", // 'slight snow': 24,
  "\U0000F015", // 'hail': 25,
  "\U0000F01D", // 'thunderstorn with hail': 26,
};


static char* moon_phase[] ={
  "\U0000F095",//'wi-moon-new':0,
  "\U0000F096",//'wi-moon-waxing-crescent-1',1,
  "\U0000F097",//'wi-moon-waxing-crescent-2',2,
  "\U0000F098",//'wi-moon-waxing-crescent-3',3,
  "\U0000F099",//'wi-moon-waxing-crescent-4',4,
  "\U0000F09A",//'wi-moon-waxing-crescent-5',5,
  "\U0000F09B",//'wi-moon-waxing-crescent-6',6,
  "\U0000F09C",//'wi-moon-first-quarter',7,
  "\U0000F09D",//'wi-moon-waxing-gibbous-1',8,
  "\U0000F09E",//'wi-moon-waxing-gibbous-2',9,
  "\U0000F09F",//'wi-moon-waxing-gibbous-3',10,
  "\U0000F0A0",//'wi-moon-waxing-gibbous-4',11,
  "\U0000F0A1",//'wi-moon-waxing-gibbous-5',12,
  "\U0000F0A2",//'wi-moon-waxing-gibbous-6',13,
  "\U0000F0A3",//'wi-moon-full',14,
  "\U0000F0A4",//'wi-moon-waning-gibbous-1',15,
  "\U0000F0A5",//'wi-moon-waning-gibbous-2',16,
  "\U0000F0A6",//'wi-moon-waning-gibbous-3',17,
  "\U0000F0A7",//'wi-moon-waning-gibbous-4',18,
  "\U0000F0A8",//'wi-moon-waning-gibbous-5',19,
  "\U0000F0A9",//'wi-moon-waning-gibbous-6',20,
  "\U0000F0AA",//'wi-moon-third-quarter',21,
  "\U0000F0AB",//'wi-moon-waning-crescent-1',22,
  "\U0000F0AC",//'wi-moon-waning-crescent-2',23,
  "\U0000F0AD",//'wi-moon-waning-crescent-3',24,
  "\U0000F0AE",//'wi-moon-waning-crescent-4',25,
  "\U0000F0AF",//'wi-moon-waning-crescent-5',26,
  "\U0000F0B0",//'wi-moon-waning-crescent-6',27,
  "\U0000F095",//'wi-moon-new',28,
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
  settings.Text10Color = GColorWhite;
  settings.HourColor = GColorWhite;
  settings.MinColor = GColorWhite;
  settings.DateColor = GColorWhite;
  settings.WeatherUnit = 0;
  settings.UseWeather = false;
  //settings.WindUnit = 0;
  //settings.RainUnit = 0;
  settings.UpSlider = 30;
  settings.NightTheme = false;
  settings.HealthOff = true;
  settings.VibeOn = false;
  settings.AddZero12h = false;
  settings.RemoveZero24h = false;
  }
int HourSunrise=700;
int HourSunset=2200;
int moonphase=0;
bool BTOn=true;
bool GPSOn=true;
bool IsNightNow=false;
bool showWeather = false;
int s_countdown = 0;
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
  BTOn = connected;
}

static void bluetooth_callback2(bool connected2){
  BTOn = connected2;
}


static void bluetooth_vibe (bool connected) {
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
  if(!quiet_time_is_active()) {
  layer_set_hidden(s_canvas_qt_icon,true);
  }
}


static AppTimer *s_timeout_timer;


static void  timeout_handler(void *context) {


showWeather = false;
layer_set_hidden (time_area_layer, showWeather);
layer_set_hidden (s_canvas, showWeather);
layer_set_hidden (time_area_layer_weather, !showWeather);
layer_set_hidden (s_canvas_weather, !showWeather);
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
  showWeather = true;
  if(showWeather && settings.UseWeather){
  layer_set_hidden (time_area_layer, showWeather);
  layer_set_hidden (s_canvas, showWeather);
  layer_set_hidden (time_area_layer_weather, !showWeather);
  layer_set_hidden (s_canvas_weather, !showWeather);
  layer_set_hidden (s_canvas_bt_icon, showWeather);
  layer_set_hidden (s_canvas_qt_icon, showWeather);
  }
  else{
    layer_set_hidden (time_area_layer, !showWeather);
    layer_set_hidden (s_canvas, !showWeather);
    layer_set_hidden (time_area_layer_weather, showWeather);
    layer_set_hidden (s_canvas_weather, showWeather);
    layer_set_hidden (s_canvas_bt_icon, !showWeather);
    layer_set_hidden (s_canvas_qt_icon, !showWeather);
    }


  s_timeout_timer = app_timer_register(10000, timeout_handler,NULL);
  //10 seconds


}
















void layer_update_proc_background (Layer * back_layer, GContext * ctx){
  // Create Rects
  GRect bounds = layer_get_bounds(back_layer);


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


  FContext fctx;


  fctx_init_context(&fctx, ctx);
  fctx_set_color_bias(&fctx, 0);


  #ifdef PBL_MICROPHONE
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
  snprintf(daydraw, sizeof(daydraw), "%d", daynow);
  //snprintf(daydraw, sizeof(daydraw), "%s", "27");


  //draw hours
  fctx_set_fill_color(&fctx, ColorSelect(settings.HourColor, settings.HourColorN));


  FPoint hour_pos;
  fctx_begin_fill(&fctx);
  fctx_set_text_em_height(&fctx, time_font, font_size_hour);


  hour_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90, 143) );


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


  min_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90, 143));
  if (bounds.size.h == boundsobs.size.h)
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(84, (boundsobs.size.h/4*3)-6 ));
     fctx_set_text_em_height(&fctx, time_font, font_size_min);
  }
  else
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(84, (144/2)+12 ));
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


  date_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(24, 0));
  date_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(90-24+2, 6+16));


  fctx_set_offset(&fctx, date_pos);


  fctx_draw_string(&fctx, daydraw, time_font, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft), PBL_IF_ROUND_ELSE(FTextAnchorMiddle, FTextAnchorTop));
  fctx_end_fill(&fctx);


  fctx_deinit_context(&fctx);
}


void update_time_area_layer_weather(Layer *l, GContext* ctx) {
    // check layer bounds
  #ifdef PBL_ROUND
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


  hour_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(138, 143) );


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


  min_pos.x = INT_TO_FIXED(PBL_IF_ROUND_ELSE(138, 143));
  if (bounds.size.h == boundsobs.size.h)
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(92-5 , (boundsobs.size.h/4*3)-6 ));
     fctx_set_text_em_height(&fctx, time_font, font_size_min);
  }
  else
  {
     min_pos.y = INT_TO_FIXED(PBL_IF_ROUND_ELSE(92-5 , (144/2)+12 ));
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
//add in weather info
GRect DateRect =
  (PBL_IF_ROUND_ELSE(
     GRect(2, 44, 44, 20),
     GRect(1, 4, 48, 24)));


GRect BatteryRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,180,2),
      GRect(0,0,144,3)));


    //Battery
  int s_battery_level = battery_state_service_peek().charge_percent;


#ifdef PBL_ROUND
  int width_round = (s_battery_level * 180) / 100;
#else
  int width_rect = (s_battery_level * 144) / 100;
#endif


GRect BatteryFillRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,width_round,2),
      GRect(0,0,width_rect,3)));


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


static void layer_update_proc_weather(Layer * layer, GContext * ctx){




//add in weather info
GRect SunsetRect =
  (PBL_IF_ROUND_ELSE(
     GRect(0,133-8-37,54,14),
     GRect(96-25-4,144+3-6,144/3,27)));


GRect SunriseRect =
   (PBL_IF_ROUND_ELSE(
     GRect(0,133-8-37-48+2+24,54,14),
      GRect(96-25-4+2,144+3-21-6-3,144/3,27)));


GRect SunsetIconRect =
  (PBL_IF_ROUND_ELSE(
    GRect(0,150-3-36,54,24),
    GRect(72+47,143,144/6,38)));


GRect SunriseIconRect =
   (PBL_IF_ROUND_ELSE(
     GRect(0,150-3-36-48+2-14,54,24),
     GRect(72+47,124-3-2,144/6,38)));


//GRect MoonRect =
//    (PBL_IF_ROUND_ELSE(
//      GRect(90-9-27+29-4,132-5+12-37+1-2,18+54,56),
//      GRect(28, 127, 56,56)));


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


/*GRect BatteryRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,180,2),
      GRect(0,0,144,3)));


    //Battery
  int s_battery_level = battery_state_service_peek().charge_percent;


#ifdef PBL_ROUND
  int width_round = (s_battery_level * 180) / 100;
#else
  int width_rect = (s_battery_level * 144) / 100;
#endif


GRect BatteryFillRect =
    (PBL_IF_ROUND_ELSE(
      GRect(0,88,width_round,2),
      GRect(0,0,width_rect,3)));


  char battperc[6];
  snprintf(battperc, sizeof(battperc), "%d", s_battery_level);
  strcat(battperc, "%");
*/

 //Date
 // Local language
 /*const char * sys_locale = i18n_get_system_locale();
 char datedraw[6];
 fetchwday(s_weekday, sys_locale, datedraw);
 char datenow[6];
 snprintf(datenow, sizeof(datenow), "%s", datedraw);*/


 //draw day of the week
// graphics_context_set_text_color(ctx, ColorSelect(settings.Text3Color, settings.Text3ColorN));
// graphics_draw_text(ctx, datenow, FontDayOfTheWeekShortName, DateRect, GTextOverflowModeWordWrap, PBL_IF_ROUND_ELSE(GTextAlignmentRight,GTextAlignmentLeft), NULL);


 // Draw the battery bar background
 /*graphics_context_set_fill_color(ctx, ColorSelect(settings.FrameColor2,settings.FrameColor2N));// GColorBlack);
 graphics_fill_rect(ctx, BatteryRect, 0, GCornerNone);


 // Draw the battery bar
 graphics_context_set_fill_color(ctx, ColorSelect(settings.Text6Color, settings.Text6ColorN));
 graphics_fill_rect(ctx,BatteryFillRect, 0, GCornerNone);*/


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






 //char MoonToDraw[4];
 //snprintf(MoonToDraw, sizeof(MoonToDraw), "%s",settings.moonstring);
 //graphics_context_set_text_color(ctx,ColorSelect(settings.Text4Color,settings.Text4ColorN));
 //graphics_draw_text(ctx, MoonToDraw, FontWeatherCondition, MoonRect, GTextOverflowModeFill, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentCenter), NULL);






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


  GRect BTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(180-26,90-24,24,20),
      GRect(1,52+6,48,20)));


 graphics_context_set_text_color(ctx3, ColorSelect(settings.Text5Color, settings.Text5ColorN));
 graphics_draw_text(ctx3, "z", FontBTQTIcons, BTIconRect, GTextOverflowModeFill,PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft), NULL);


}


static void layer_update_proc_qt(Layer * layer4, GContext * ctx4){
  //layer_set_hidden(s_canvas_qt_icon, showWeather);


  GRect QTIconRect =
    (PBL_IF_ROUND_ELSE(
      GRect(180-26,92,24,20),
      GRect(1,168-41,48,20)));


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


  Tuple * moon_tuple = dict_find(iter, MESSAGE_KEY_MoonPhase);
  if (moon_tuple){
    snprintf(settings.moonstring,sizeof(settings.moonstring),"%s",moon_phase[(int)moon_tuple->value->int32]);
  }


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
  layer_mark_dirty(s_canvas_bt_icon);
  layer_mark_dirty(s_canvas_qt_icon);


  prv_save_settings();
}


//Load main layer
static void window_load(Window * window){
  Layer * window_layer = window_get_root_layer(window);
  GRect bounds4 = layer_get_bounds(window_layer);


  s_canvas_background = layer_create(bounds4);
    layer_set_update_proc(s_canvas_background, layer_update_proc_background);
    layer_add_child(window_layer, s_canvas_background);


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


    time_area_layer = layer_create(bounds4);
       layer_add_child(window_layer, time_area_layer);
       layer_set_update_proc(time_area_layer, update_time_area_layer);


     time_area_layer_weather = layer_create(bounds4);
        layer_set_hidden (time_area_layer_weather, !showWeather);
        layer_add_child(window_layer, time_area_layer_weather);
        layer_set_update_proc(time_area_layer_weather, update_time_area_layer_weather);


     s_canvas_weather = layer_create(bounds4);
        layer_set_hidden (s_canvas_weather, !showWeather);
        layer_set_update_proc(s_canvas_weather, layer_update_proc_weather);
        layer_add_child(window_layer, s_canvas_weather);


  s_canvas = layer_create(bounds4);
     layer_set_update_proc(s_canvas, layer_update_proc);
     layer_add_child(window_layer, s_canvas);
  }




static void window_unload(Window * window){
  layer_destroy (s_canvas_background);
  layer_destroy(time_area_layer);
  layer_destroy(time_area_layer_weather);
  layer_destroy(s_canvas_bt_icon);
  layer_destroy(s_canvas);
  layer_destroy(s_canvas_weather);
  layer_destroy(s_canvas_qt_icon);
  window_destroy(s_window);


  ffont_destroy(time_font);
  fonts_unload_custom_font(FontDayOfTheWeekShortName);
  fonts_unload_custom_font(FontBTQTIcons);
  fonts_unload_custom_font(FontTemp);
  fonts_unload_custom_font(FontTempFore);
  fonts_unload_custom_font(FontWeatherIcons);
  fonts_unload_custom_font(FontWeatherCondition);


}


void main_window_push(){
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}


void main_window_update(int hours, int minutes, int weekday, int day){
  s_hours = hours;
  s_minutes = minutes;
  s_day = day;
  s_weekday = weekday;


  layer_mark_dirty(time_area_layer);
  layer_mark_dirty(time_area_layer_weather);
}


static void tick_handler(struct tm * time_now, TimeUnits changed){


  main_window_update(time_now -> tm_hour, time_now -> tm_min, time_now -> tm_wday, time_now -> tm_mday);
  //update_time();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick at %d", time_now -> tm_min);
  //s_loop = 0;
  if (s_countdown == 0){
    //Reset
    s_countdown = settings.UpSlider;
  } else{
    s_countdown = s_countdown - 1;
  }
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Countdown to update %d loop is %d, GPS is %d", s_countdown, s_loop, GPSOn);




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
    //  if (settings.UpSlider > 15){
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
    //  }
    }


  //onreconnection(BTOn, connection_service_peek_pebble_app_connection());
  if (!BTOn && connection_service_peek_pebble_app_connection()){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BT reconnected, requesting weather at %d", time_now -> tm_min);
    s_loop = 0;
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


static void init(){


  prv_load_settings();


  // Listen for AppMessages


  s_countdown = settings.UpSlider;
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


  FontDayOfTheWeekShortName = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_12, RESOURCE_ID_FONT_LIZ_18)));
  FontBTQTIcons = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_DRIPICONS_16));
  FontTemp = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_27, RESOURCE_ID_FONT_LIZ_27)));
  FontTempFore = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_LIZ_18, RESOURCE_ID_FONT_LIZ_18)));
//sunset sunrise icon font
  FontWeatherIcons = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_16,RESOURCE_ID_FONT_WEATHERICONS_16)));
//weather icons
  FontWeatherCondition = fonts_load_custom_font(resource_get_handle(PBL_IF_ROUND_ELSE(RESOURCE_ID_FONT_WEATHERICONS_32,RESOURCE_ID_FONT_WEATHERICONS_32)));


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
  tick_timer_service_unsubscribe();
  app_message_deregister_callbacks();
  battery_state_service_unsubscribe();
  connection_service_unsubscribe();
//  health_service_events_unsubscribe();
  accel_tap_service_unsubscribe();
}
int main(){
  init();
  app_event_loop();
  deinit();
}
