
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" \
                            issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void *data);
static void memory_monitor(lv_task_t *param);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t *kb_indev;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

LV_FONT_DECLARE(jetbrains_mono_extrabold_compressed)
LV_FONT_DECLARE(jetbrains_mono_bold_20)



lv_theme_t theme;
lv_style_t def;
lv_style_t scr, bg, sb, panel;
lv_font_t * font = NULL;
uint16_t hue = 10;
lv_theme_t theme;
lv_style_t btn_rel, btn_pr, btn_tgl_rel, btn_tgl_pr, btn_ina;
lv_style_t labelBigStyle;
lv_style_t prim, sec, hint;
lv_style_t led;
lv_style_t bar_bg, bar_indic;
lv_style_t slider_knob;
lv_style_t arc;
lv_style_t cal_bg;
lv_style_t cal_header;
lv_style_t week_box;
lv_style_t today_box;
lv_style_t highlighted_days;
lv_style_t ina_days;
lv_style_t rel, pr, tgl_rel, tgl_pr, ina;
lv_style_t btnm_bg, btnm_rel, btnm_pr, btnm_tgl_rel, btnm_tgl_pr, btnm_ina;
lv_style_t mbox_bg;
lv_style_t page_scrl;
lv_style_t list_bg, list_btn_rel, list_btn_pr, list_btn_tgl_rel, list_btn_tgl_pr;
lv_style_t ddlist_bg, ddlist_sel;
lv_style_t cell;
lv_style_t win_bg;
lv_style_t win_header;
lv_style_t win_btn_pr;
void InitBaseTheme() {
  if(font == NULL) font = &jetbrains_mono_bold_20;
  lv_style_copy(&def, &lv_style_plain); /*Initialize the default style*/
  def.text.font = font;

  lv_style_copy(&bg, &lv_style_plain);
  bg.body.main_color = LV_COLOR_BLACK;
  bg.body.grad_color = LV_COLOR_BLACK;
  bg.text.color      = LV_COLOR_WHITE;
  bg.text.font       = font;
  bg.image.color     = LV_COLOR_WHITE;

  lv_style_copy(&scr, &bg);
  scr.body.padding.bottom = 0;
  scr.body.padding.top    = 0;
  scr.body.padding.left   = 0;
  scr.body.padding.right  = 0;

  lv_style_copy(&sb, &def);
  sb.body.main_color     = lv_color_hsv_to_rgb(hue, 30, 60);
  sb.body.grad_color     = lv_color_hsv_to_rgb(hue, 30, 60);
  sb.body.border.width   = 0;
  sb.body.padding.inner  = LV_DPI / 20;
  sb.body.padding.left   = 0;
  sb.body.padding.right  = 0;
  sb.body.padding.top    = 0;
  sb.body.padding.bottom = 0;
  sb.body.radius         = LV_DPI / 30;
  sb.body.opa            = LV_OPA_COVER;

  lv_style_copy(&panel, &bg);
  panel.body.main_color     = lv_color_hsv_to_rgb(hue, 11, 18);
  panel.body.grad_color     = lv_color_hsv_to_rgb(hue, 11, 18);
  panel.body.radius         = LV_DPI / 20;
  panel.body.border.color   = lv_color_hsv_to_rgb(hue, 10, 25);
  panel.body.border.width   = 1;
  panel.body.border.opa     = LV_OPA_COVER;
  panel.body.padding.left   = LV_DPI / 10;
  panel.body.padding.right  = LV_DPI / 10;
  panel.body.padding.top    = LV_DPI / 10;
  panel.body.padding.bottom = LV_DPI / 10;
  panel.line.color          = lv_color_hsv_to_rgb(hue, 20, 40);
  panel.line.width          = 1;

  theme.style.scr   = &scr;
  theme.style.bg    = &bg;
  theme.style.panel = &def;
}

void InitThemeContainer() {
  theme.style.cont = &panel;
}

void InitThemeButton() {


  lv_style_copy(&btn_rel, &def);
  btn_rel.body.main_color     = lv_color_hsv_to_rgb(hue, 10, 40);
  btn_rel.body.grad_color     = lv_color_hsv_to_rgb(hue, 10, 20);
  btn_rel.body.border.color   = lv_color_hex3(0x111);
  btn_rel.body.border.width   = 1;
  btn_rel.body.border.opa     = LV_OPA_70;
  btn_rel.body.padding.left   = LV_DPI / 4;
  btn_rel.body.padding.right  = LV_DPI / 4;
  btn_rel.body.padding.top    = LV_DPI / 8;
  btn_rel.body.padding.bottom = LV_DPI / 8;
  btn_rel.body.shadow.type    = LV_SHADOW_BOTTOM;
  btn_rel.body.shadow.color   = lv_color_hex3(0x111);
  btn_rel.body.shadow.width   = LV_DPI / 30;
  btn_rel.text.color          = lv_color_hex3(0xeee);
  btn_rel.image.color         = lv_color_hex3(0xeee);

  lv_style_copy(&btn_pr, &btn_rel);
  btn_pr.body.main_color = lv_color_hsv_to_rgb(hue, 10, 30);
  btn_pr.body.grad_color = lv_color_hsv_to_rgb(hue, 10, 10);

  lv_style_copy(&btn_tgl_rel, &btn_rel);
  btn_tgl_rel.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 20);
  btn_tgl_rel.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 40);
  btn_tgl_rel.body.shadow.width = LV_DPI / 40;
  btn_tgl_rel.text.color        = lv_color_hex3(0xddd);
  btn_tgl_rel.image.color       = lv_color_hex3(0xddd);

  lv_style_copy(&btn_tgl_pr, &btn_rel);
  btn_tgl_pr.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 10);
  btn_tgl_pr.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 30);
  btn_tgl_pr.body.shadow.width = LV_DPI / 30;
  btn_tgl_pr.text.color        = lv_color_hex3(0xddd);
  btn_tgl_pr.image.color       = lv_color_hex3(0xddd);

  lv_style_copy(&btn_ina, &btn_rel);
  btn_ina.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 20);
  btn_ina.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 20);
  btn_ina.body.shadow.width = 0;
  btn_ina.text.color        = lv_color_hex3(0xaaa);
  btn_ina.image.color       = lv_color_hex3(0xaaa);

  theme.style.btn.rel     = &btn_rel;
  theme.style.btn.pr      = &btn_pr;
  theme.style.btn.tgl_rel = &btn_tgl_rel;
  theme.style.btn.tgl_pr  = &btn_tgl_pr;
  theme.style.btn.ina     = &btn_ina;
}


lv_style_t* LabelBigStyle = NULL;
void InitThemeLabel() {
  lv_style_copy(&prim, &bg);
  prim.text.color = lv_color_hsv_to_rgb(hue, 5, 95);

  lv_style_copy(&labelBigStyle, &prim);
  labelBigStyle.text.font = &jetbrains_mono_extrabold_compressed;
  LabelBigStyle = &(labelBigStyle);

  lv_style_copy(&sec, &bg);
  sec.text.color = lv_color_hsv_to_rgb(hue, 15, 65);

  lv_style_copy(&hint, &bg);
  hint.text.color = lv_color_hsv_to_rgb(hue, 20, 55);

  theme.style.label.prim = &prim;
  theme.style.label.sec  = &sec;
  theme.style.label.hint = &hint;
}

void InitThemeLine() {
  theme.style.line.decor = &def;
}

void InitThemeLed() {
  lv_style_copy(&led, &def);
  led.body.shadow.width = LV_DPI / 10;
  led.body.radius       = LV_RADIUS_CIRCLE;
  led.body.border.width = LV_DPI / 30;
  led.body.border.opa   = LV_OPA_30;
  led.body.main_color   = lv_color_hsv_to_rgb(hue, 100, 100);
  led.body.grad_color   = lv_color_hsv_to_rgb(hue, 100, 40);
  led.body.border.color = lv_color_hsv_to_rgb(hue, 60, 60);
  led.body.shadow.color = lv_color_hsv_to_rgb(hue, 100, 100);

  theme.style.led = &led;
}

void InitThemeImage() {
  theme.style.img.light = &def;
  theme.style.img.dark  = &def;
}

void InitThemeBar() {
  lv_style_copy(&bar_bg, &panel);
  bar_bg.body.padding.left   = LV_DPI / 16;
  bar_bg.body.padding.right  = LV_DPI / 16;
  bar_bg.body.padding.top    = LV_DPI / 16;
  bar_bg.body.padding.bottom = LV_DPI / 16;
  bar_bg.body.radius         = LV_RADIUS_CIRCLE;

  lv_style_copy(&bar_indic, &def);
  bar_indic.body.main_color     = lv_color_hsv_to_rgb(hue, 80, 70);
  bar_indic.body.grad_color     = lv_color_hsv_to_rgb(hue, 80, 70);
  bar_indic.body.border.color   = lv_color_hsv_to_rgb(hue, 20, 15);
  bar_indic.body.border.width   = 1;
  bar_indic.body.border.opa     = LV_OPA_COVER;
  bar_indic.body.radius         = LV_RADIUS_CIRCLE;
  bar_indic.body.padding.left   = 0;
  bar_indic.body.padding.right  = 0;
  bar_indic.body.padding.top    = 0;
  bar_indic.body.padding.bottom = 0;

  theme.style.bar.bg    = &bar_bg;
  theme.style.bar.indic = &bar_indic;
}

void InitThemeSlider() {
  lv_style_copy(&slider_knob, theme.style.btn.rel);
  slider_knob.body.radius = LV_RADIUS_CIRCLE;

  theme.style.slider.bg    = theme.style.bar.bg;
  theme.style.slider.indic = theme.style.bar.indic;
  theme.style.slider.knob  = &slider_knob;
}

void InitThemeSwitch() {
  theme.style.sw.bg       = theme.style.bar.bg;
  theme.style.sw.indic    = theme.style.bar.indic;
  theme.style.sw.knob_off = theme.style.slider.knob;
  theme.style.sw.knob_on  = theme.style.slider.knob;
}

void InitThemeMeter() {
  static lv_style_t lmeter_bg;
  lv_style_copy(&lmeter_bg, &def);
  lmeter_bg.body.main_color    = lv_color_hsv_to_rgb(hue, 10, 70);
  lmeter_bg.body.grad_color    = lv_color_hsv_to_rgb(hue, 95, 90);
  lmeter_bg.body.padding.left  = LV_DPI / 10; /*Scale line length*/
  lmeter_bg.body.padding.inner = LV_DPI / 10; /*Text padding*/
  lmeter_bg.body.border.color  = lv_color_hex3(0x333);
  lmeter_bg.line.color         = lv_color_hex3(0x555);
  lmeter_bg.line.width         = 1;
  lmeter_bg.text.color         = lv_color_hex3(0xddd);

  theme.style.lmeter = &lmeter_bg;
}

void InitThemeGauge() {
  static lv_style_t gauge_bg;
  lv_style_copy(&gauge_bg, &def);
  gauge_bg.body.main_color = lv_color_hsv_to_rgb(hue, 10, 70);
  gauge_bg.body.grad_color = gauge_bg.body.main_color;
  gauge_bg.line.color      = lv_color_hsv_to_rgb(hue, 80, 75);
  gauge_bg.line.width      = 1;
  gauge_bg.text.color      = lv_color_hex3(0xddd);

  theme.style.gauge = &gauge_bg;
}

void InitThemeArc() {
  lv_style_copy(&arc, &def);
  arc.line.width   = 8;
  arc.line.color   = lv_color_hsv_to_rgb(hue, 80, 70);
  arc.line.rounded = 1;

  /*For preloader*/
  arc.body.border.width   = 7;
  arc.body.border.color   = lv_color_hsv_to_rgb(hue, 11, 48);
  arc.body.padding.left   = 1;
  arc.body.padding.right  = 1;
  arc.body.padding.top    = 1;
  arc.body.padding.bottom = 1;

  theme.style.arc = &arc;
}

void InitThemePreload() {
//  theme.style.preload = theme.style.arc;
}

void InitThemeChart() {
  theme.style.chart = &panel;
}

void InitThemeCalendar() {

  lv_style_copy(&cal_bg, &bg);
  cal_bg.body.main_color     = lv_color_hsv_to_rgb(hue, 10, 40);
  cal_bg.body.grad_color     = lv_color_hsv_to_rgb(hue, 10, 40);
  cal_bg.body.border.color   = lv_color_hex3(0x333);
  cal_bg.body.border.width   = 1;
  cal_bg.body.radius         = LV_DPI / 20;
  cal_bg.body.padding.left   = LV_DPI / 10;
  cal_bg.body.padding.right  = LV_DPI / 10;
  cal_bg.body.padding.top    = LV_DPI / 10;
  cal_bg.body.padding.bottom = LV_DPI / 10;


  lv_style_copy(&cal_header, &bg);
  cal_header.body.main_color     = lv_color_hsv_to_rgb(hue, 10, 20);
  cal_header.body.grad_color     = lv_color_hsv_to_rgb(hue, 10, 20);
  cal_header.body.radius         = 0;
  cal_header.body.border.width   = 1;
  cal_header.body.border.color   = lv_color_hex3(0x333);
  cal_header.body.padding.left   = LV_DPI / 10;
  cal_header.body.padding.right  = LV_DPI / 10;
  cal_header.body.padding.top    = LV_DPI / 10;
  cal_header.body.padding.bottom = LV_DPI / 10;


  lv_style_copy(&week_box, &panel);
  week_box.body.main_color     = lv_color_hsv_to_rgb(hue, 30, 45);
  week_box.body.grad_color     = lv_color_hsv_to_rgb(hue, 30, 45);
  week_box.body.radius         = LV_DPI / 20;
  week_box.body.border.width   = 1;
  week_box.body.padding.left   = LV_DPI / 20;
  week_box.body.padding.right  = LV_DPI / 20;
  week_box.body.padding.top    = LV_DPI / 25;
  week_box.body.padding.bottom = LV_DPI / 25;

  lv_style_copy(&today_box, &week_box);
  today_box.body.main_color     = lv_color_hsv_to_rgb(hue, 80, 70);
  today_box.body.grad_color     = lv_color_hsv_to_rgb(hue, 80, 70);
  today_box.body.radius         = LV_DPI / 20;
  today_box.body.padding.left   = LV_DPI / 14;
  today_box.body.padding.right  = LV_DPI / 14;
  today_box.body.padding.top    = LV_DPI / 14;
  today_box.body.padding.bottom = LV_DPI / 14;

  lv_style_copy(&highlighted_days, &bg);
  highlighted_days.text.color = lv_color_hsv_to_rgb(hue, 40, 80);

  lv_style_copy(&ina_days, &bg);
  ina_days.text.color = lv_color_hsv_to_rgb(hue, 0, 60);

  theme.style.calendar.bg               = &cal_bg;
  theme.style.calendar.header           = &cal_header;
  theme.style.calendar.week_box         = &week_box;
  theme.style.calendar.today_box        = &today_box;
  theme.style.calendar.highlighted_days = &highlighted_days;
  theme.style.calendar.day_names        = &cal_bg;
  theme.style.calendar.inactive_days    = &ina_days;
}

void InitThemeCheckBox() {

  lv_style_copy(&rel, &def);
  rel.body.radius       = LV_DPI / 20;
  rel.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 95);
  rel.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 95);
  rel.body.border.color = lv_color_hsv_to_rgb(hue, 10, 50);
  rel.body.border.width = 2;
  ;

  lv_style_copy(&pr, &rel);
  pr.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 80);
  pr.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 80);
  pr.body.border.color = lv_color_hsv_to_rgb(hue, 10, 20);
  pr.body.border.width = 1;
  ;

  lv_style_copy(&tgl_rel, &rel);
  tgl_rel.body.main_color   = lv_color_hsv_to_rgb(hue, 80, 90);
  tgl_rel.body.grad_color   = lv_color_hsv_to_rgb(hue, 80, 90);
  tgl_rel.body.border.color = lv_color_hsv_to_rgb(hue, 80, 50);

  lv_style_copy(&tgl_pr, &tgl_rel);
  tgl_pr.body.main_color   = lv_color_hsv_to_rgb(hue, 80, 70);
  tgl_pr.body.grad_color   = lv_color_hsv_to_rgb(hue, 80, 70);
  tgl_pr.body.border.color = lv_color_hsv_to_rgb(hue, 80, 30);
  tgl_pr.body.border.width = 1;
  ;

  lv_style_copy(&ina, &rel);
  ina.body.main_color   = lv_color_hex3(0x777);
  ina.body.grad_color   = lv_color_hex3(0x777);
  ina.body.border.width = 0;

  theme.style.cb.bg          = &lv_style_transp;
  theme.style.cb.box.rel     = &rel;
  theme.style.cb.box.pr      = &pr;
  theme.style.cb.box.tgl_rel = &tgl_rel;
  theme.style.cb.box.tgl_pr  = &tgl_pr;
  theme.style.cb.box.ina     = &def;
}

void InitThemeButtonMatrix() {

  lv_style_copy(&btnm_bg, theme.style.btn.rel);
  btnm_bg.body.padding.left   = 2;
  btnm_bg.body.padding.right  = 2;
  btnm_bg.body.padding.top    = 2;
  btnm_bg.body.padding.bottom = 2;
  btnm_bg.body.padding.inner  = 0;
  btnm_bg.body.border.width   = 1;

  lv_style_copy(&btnm_rel, theme.style.btn.rel);
  btnm_rel.body.border.part  = LV_BORDER_FULL | LV_BORDER_INTERNAL;
  btnm_rel.body.border.width = 1;
  btnm_rel.body.radius       = 2;

  lv_style_copy(&btnm_pr, theme.style.btn.pr);
  btnm_pr.body.border.part  = btnm_rel.body.border.part;
  btnm_pr.body.border.width = btnm_rel.body.border.width;
  btnm_pr.body.radius       = btnm_rel.body.radius;

  lv_style_copy(&btnm_tgl_rel, theme.style.btn.tgl_rel);
  btnm_tgl_rel.body.border.part  = btnm_rel.body.border.part;
  btnm_tgl_rel.body.border.width = btnm_rel.body.border.width;
  btnm_tgl_rel.body.radius       = btnm_rel.body.radius;

  lv_style_copy(&btnm_tgl_pr, theme.style.btn.pr);
  btnm_tgl_pr.body.border.part  = btnm_rel.body.border.part;
  btnm_tgl_pr.body.border.width = btnm_rel.body.border.width;
  btnm_tgl_pr.body.radius       = btnm_rel.body.radius;

  lv_style_copy(&btnm_ina, theme.style.btn.ina);
  btnm_ina.body.border.part  = btnm_rel.body.border.part;
  btnm_ina.body.border.width = btnm_rel.body.border.width;
  btnm_ina.body.radius       = btnm_rel.body.radius;

  theme.style.btnm.bg          = &btnm_bg;
  theme.style.btnm.btn.rel     = &btnm_rel;
  theme.style.btnm.btn.pr      = &btnm_pr;
  theme.style.btnm.btn.tgl_rel = &btnm_tgl_rel;
  theme.style.btnm.btn.tgl_pr  = &btnm_tgl_pr;
  theme.style.btnm.btn.ina     = &btnm_ina;
}

void InitThemeKnob() {
  theme.style.kb.bg          = &bg;
  theme.style.kb.btn.rel     = theme.style.btn.rel;
  theme.style.kb.btn.pr      = theme.style.btn.pr;
  theme.style.kb.btn.tgl_rel = theme.style.btn.tgl_rel;
  theme.style.kb.btn.tgl_pr  = theme.style.btn.tgl_pr;
  theme.style.kb.btn.ina     = theme.style.btn.ina;
}

void InitThemeMessageBox() {
  lv_style_copy(&mbox_bg, &bg);
  mbox_bg.body.main_color   = lv_color_hsv_to_rgb(hue, 30, 30);
  mbox_bg.body.grad_color   = lv_color_hsv_to_rgb(hue, 30, 30);
  mbox_bg.body.border.color = lv_color_hsv_to_rgb(hue, 11, 20);
  mbox_bg.body.border.width = 1;
  mbox_bg.body.shadow.width = LV_DPI / 10;
  mbox_bg.body.shadow.color = lv_color_hex3(0x222);
  mbox_bg.body.radius       = LV_DPI / 20;
  theme.style.mbox.bg       = &mbox_bg;
  theme.style.mbox.btn.bg   = &lv_style_transp;
  theme.style.mbox.btn.rel  = theme.style.btn.rel;
  theme.style.mbox.btn.pr   = theme.style.btn.pr;
}

void InitThemePage() {
  lv_style_copy(&page_scrl, &bg);
  page_scrl.body.main_color   = lv_color_hsv_to_rgb(hue, 10, 40);
  page_scrl.body.grad_color   = lv_color_hsv_to_rgb(hue, 10, 40);
  page_scrl.body.border.color = lv_color_hex3(0x333);
  page_scrl.body.border.width = 1;
  page_scrl.body.radius       = LV_DPI / 20;

  theme.style.page.bg   = &panel;
  theme.style.page.scrl = &page_scrl;
  theme.style.page.sb   = &sb;
}

void InitThemeTextArea() {
  theme.style.ta.area    = &panel;
  theme.style.ta.oneline = &panel;
  theme.style.ta.cursor  = NULL;
  theme.style.ta.sb      = &def;
}

void InitThemeSpinBox() {
  theme.style.spinbox.bg     = &panel;
  theme.style.spinbox.cursor = theme.style.ta.cursor;
  theme.style.spinbox.sb     = theme.style.ta.sb;
}

void InitThemeList() {

  lv_style_copy(&list_bg, &panel);
  list_bg.body.padding.top    = 0;
  list_bg.body.padding.bottom = 0;
  list_bg.body.padding.left   = 0;
  list_bg.body.padding.right  = 0;
  list_bg.body.padding.inner  = 0;

  lv_style_copy(&list_btn_rel, &bg);
  list_btn_rel.body.opa            = LV_OPA_TRANSP;
  list_btn_rel.body.border.part    = LV_BORDER_BOTTOM;
  list_btn_rel.body.border.color   = lv_color_hsv_to_rgb(hue, 10, 5);
  list_btn_rel.body.border.width   = 1;
  list_btn_rel.body.radius         = LV_DPI / 10;
  list_btn_rel.text.color          = lv_color_hsv_to_rgb(hue, 5, 80);
  list_btn_rel.image.color         = lv_color_hsv_to_rgb(hue, 5, 80);
  list_btn_rel.body.padding.top    = LV_DPI / 6;
  list_btn_rel.body.padding.bottom = LV_DPI / 6;
  list_btn_rel.body.padding.left   = LV_DPI / 8;
  list_btn_rel.body.padding.right  = LV_DPI / 8;

  lv_style_copy(&list_btn_pr, theme.style.btn.pr);
  list_btn_pr.body.main_color     = lv_color_hsv_to_rgb(hue, 10, 5);
  list_btn_pr.body.grad_color     = lv_color_hsv_to_rgb(hue, 10, 5);
  list_btn_pr.body.border.color   = lv_color_hsv_to_rgb(hue, 10, 5);
  list_btn_pr.body.border.width   = 0;
  list_btn_pr.body.padding.top    = LV_DPI / 6;
  list_btn_pr.body.padding.bottom = LV_DPI / 6;
  list_btn_pr.body.padding.left   = LV_DPI / 8;
  list_btn_pr.body.padding.right  = LV_DPI / 8;
  list_btn_pr.text.color          = lv_color_hsv_to_rgb(hue, 5, 80);
  list_btn_pr.image.color         = lv_color_hsv_to_rgb(hue, 5, 80);

  lv_style_copy(&list_btn_tgl_rel, &list_btn_rel);
  list_btn_tgl_rel.body.opa        = LV_OPA_COVER;
  list_btn_tgl_rel.body.main_color = lv_color_hsv_to_rgb(hue, 80, 70);
  list_btn_tgl_rel.body.grad_color = lv_color_hsv_to_rgb(hue, 80, 70);
  list_btn_tgl_rel.body.border.color   = lv_color_hsv_to_rgb(hue, 60, 40);
  list_btn_tgl_rel.body.radius         = list_bg.body.radius;

  lv_style_copy(&list_btn_tgl_pr, &list_btn_tgl_rel);
  list_btn_tgl_pr.body.main_color = lv_color_hsv_to_rgb(hue, 80, 60);
  list_btn_tgl_pr.body.grad_color = lv_color_hsv_to_rgb(hue, 80, 60);

  theme.style.list.sb          = &sb;
  theme.style.list.bg          = &list_bg;
  theme.style.list.scrl        = &lv_style_transp_tight;
  theme.style.list.btn.rel     = &list_btn_rel;
  theme.style.list.btn.pr      = &list_btn_pr;
  theme.style.list.btn.tgl_rel = &list_btn_tgl_rel;
  theme.style.list.btn.tgl_pr  = &list_btn_tgl_pr;
  theme.style.list.btn.ina     = &def;
}

void InitThemeDropDownList() {
  lv_style_copy(&ddlist_bg, theme.style.btn.rel);
  ddlist_bg.text.line_space     = LV_DPI / 8;
  ddlist_bg.body.padding.top    = LV_DPI / 8;
  ddlist_bg.body.padding.bottom = LV_DPI / 8;
  ddlist_bg.body.padding.left   = LV_DPI / 8;
  ddlist_bg.body.padding.right  = LV_DPI / 8;
  ddlist_bg.body.radius         = LV_DPI / 30;

  lv_style_copy(&ddlist_sel, theme.style.btn.rel);
  ddlist_sel.body.main_color = lv_color_hsv_to_rgb(hue, 20, 50);
  ddlist_sel.body.grad_color = lv_color_hsv_to_rgb(hue, 20, 50);
  ddlist_sel.body.radius     = 0;

  theme.style.ddlist.bg  = &ddlist_bg;
  theme.style.ddlist.sel = &ddlist_sel;
  theme.style.ddlist.sb  = &def;
}

void InitThemeRoller() {
  lv_style_t roller_bg;

  lv_style_copy(&roller_bg, theme.style.ddlist.bg);
  roller_bg.body.main_color = lv_color_hsv_to_rgb(hue, 10, 20);
  roller_bg.body.grad_color = lv_color_hsv_to_rgb(hue, 10, 40);
  roller_bg.text.color      = lv_color_hsv_to_rgb(hue, 5, 70);
  roller_bg.text.opa        = LV_OPA_60;

  theme.style.roller.bg  = &roller_bg;
  theme.style.roller.sel = theme.style.ddlist.sel;
}

void InitThemeTabView() {
  theme.style.tabview.bg          = &bg;
  theme.style.tabview.indic       = &lv_style_transp;
  theme.style.tabview.btn.bg      = &lv_style_transp;
  theme.style.tabview.btn.rel     = theme.style.btn.rel;
  theme.style.tabview.btn.pr      = theme.style.btn.pr;
  theme.style.tabview.btn.tgl_rel = theme.style.btn.tgl_rel;
  theme.style.tabview.btn.tgl_pr  = theme.style.btn.tgl_pr;
}

void InitThemeTileView() {
  theme.style.tileview.bg   = &lv_style_transp_tight;
  theme.style.tileview.scrl = &lv_style_transp_tight;
  theme.style.tileview.sb   = theme.style.page.sb;
}

void InitThemeTable() {
  lv_style_copy(&cell, &panel);
  cell.body.radius         = 0;
  cell.body.border.width   = 1;
  cell.body.padding.left   = LV_DPI / 12;
  cell.body.padding.right  = LV_DPI / 12;
  cell.body.padding.top    = LV_DPI / 12;
  cell.body.padding.bottom = LV_DPI / 12;

  theme.style.table.bg   = &lv_style_transp_tight;
  theme.style.table.cell = &cell;
}
void InitThemeWindow() {}


void InitTheme() {
  uint16_t i;
  lv_style_t ** style_p = (lv_style_t **)&theme.style;
  for(i = 0; i < LV_THEME_STYLE_COUNT; i++) {
    *style_p = &def;
    style_p++;
  }

  InitBaseTheme();
  InitThemeContainer();
  InitThemeButton();
  InitThemeLabel();
  InitThemeLine();
  InitThemeLed();
  InitThemeImage();
  InitThemeBar();
  InitThemeSlider();
  InitThemeSwitch();
  InitThemeMeter();
  InitThemeGauge();
  InitThemeArc();
  InitThemePreload();
  InitThemeChart();
  InitThemeCalendar();
  InitThemeCheckBox();
  InitThemeButtonMatrix();
  InitThemeKnob();
  InitThemeMessageBox();
  InitThemePage();
  InitThemeTextArea();
  InitThemeSpinBox();
  InitThemeList();
  InitThemeDropDownList();
  InitThemeRoller();
  InitThemeTabView();
  InitThemeTileView();
  InitThemeTable();
  InitThemeWindow();

  lv_theme_set_current(&theme);
}

static  const char* none = "";
static  const char* batteryFull = "\xEF\x89\x80";
static  const char* batteryEmpty = "\xEF\x89\x84";
static  const char* batteryThreeQuarter = "\xEF\x89\x81";
static  const char* batteryHalf = "\xEF\x89\x82";
static  const char* batteryOneQuarter = "\xEF\x89\x83";
static  const char* heartBeat = "\xEF\x88\x9E";
static  const char* bluetoothFull = "\xEF\x8A\x93";
static  const char* bluetooth = "\xEF\x8A\x94";
static  const char* plug = "\xEF\x87\xA6";
static  const char* shoe = "\xEF\x95\x8B";
static  const char* clock = "\xEF\x80\x97";
static  const char* info = "\xEF\x84\xA9";
static  const char* list = "\xEF\x80\xBA";
static  const char* sun = "\xEF\x86\x85";
static  const char* check = "\xEF\x95\xA0";
static  const char* music = "\xEF\x80\x81";
static  const char* tachometer = "\xEF\x8F\xBD";
static  const char* asterisk = "\xEF\x81\xA9";
static  const char* paintbrush = "\xEF\x87\xBC";

static   lv_obj_t* label_time;
int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();
  InitTheme();
  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init();

  lv_style_t style;
  lv_color_t needle_colors[3];
  lv_obj_t * gauge1;

  uint32_t value=30;

  lv_style_copy(&style, &lv_style_pretty_color);
  style.body.main_color = LV_COLOR_CYAN;     /*Line color at the beginning*/
  style.body.grad_color =  LV_COLOR_RED;    /*Line color at the end*/
  style.body.padding.left = 10;                      /*Scale line length*/
  style.body.padding.inner = 8 ;                    /*Scale label padding*/
  style.body.border.color = lv_color_hex3(0x333);   /*Needle middle circle color*/
  style.line.width = 3;
  style.text.color = LV_COLOR_WHITE;
  style.line.color = LV_COLOR_RED;                  /*Line color after the critical value*/


  /*Describe the color for the needles*/

  needle_colors[0] = LV_COLOR_ORANGE;

  /*Create a gauge*/
  gauge1 = lv_gauge_create(lv_scr_act(), NULL);
  lv_gauge_set_style(gauge1, LV_GAUGE_STYLE_MAIN, &style);
  lv_gauge_set_needle_count(gauge1, 1, needle_colors);
  lv_obj_set_size(gauge1, 180, 180);
  lv_obj_align(gauge1, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_gauge_set_scale(gauge1, 360, 60, 0);
  lv_gauge_set_range(gauge1, 0, 59);

  /*Set the values*/
  lv_gauge_set_value(gauge1, 0, value);

  while (1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_task_handler();
    usleep(5 * 10000);
    lv_gauge_set_value(gauge1, 0, value++);
    if(value == 59) value = 0;
  }

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics
 * library
 */
static void hal_init(void) {
  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  monitor_init();

  /*Create a display buffer*/
  static lv_disp_buf_t disp_buf1;
  static lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
  lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

  /*Create a display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.buffer = &disp_buf1;
  disp_drv.flush_cb = monitor_flush;
  lv_disp_drv_register(&disp_drv);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  mouse_init();
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv); /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  indev_drv.read_cb = mouse_read;
  lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv);

  /*Set a cursor for the mouse*/
  LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj = lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
  lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/

  /* Tick init.
   * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about
   * how much time were elapsed Create an SDL thread to do this*/
  SDL_CreateThread(tick_thread, "tick", NULL);

  /* Optional:
   * Create a memory monitor task which prints the memory usage in
   * periodically.*/
  lv_task_create(memory_monitor, 5000, LV_TASK_PRIO_MID, NULL);
}

/**
 * A task to measure the elapsed time for LVGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void *data) {
  (void)data;

  while (1) {
    SDL_Delay(5);   /*Sleep for 5 millisecond*/
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }

  return 0;
}

/**
 * Print the memory usage periodically
 * @param param
 */
static void memory_monitor(lv_task_t *param) {
  (void)param; /*Unused*/

  lv_mem_monitor_t mon;
  lv_mem_monitor(&mon);
  printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n",
         (int)mon.total_size - mon.free_size, mon.used_pct, mon.frag_pct,
         (int)mon.free_biggest_size);
}
