#include "lcd.h"
#include "gpioPort.h"
#include "boardUtil.h"
#include "led.h"
#include "render.h"

volatile bool UpdateLED;

//*****************************************************************************
// LCD SPI CONSTANTS
//*****************************************************************************
#define   LCD_GPIO_BASE      GPIOA_BASE
#define   LCD_SPI_BASE       SSI0_BASE
#define   LCD_CLK_PIN        PA2
#define   LCD_MOSI_PIN       PA5
#define   LCD_CS_PIN         PA3
#define   LCD_CD_PIN				 PA4

#define   LCD_CLK_PIN_PCTL    GPIO_PCTL_PA2_SSI0CLK
#define   LCD_CS_PIN_PCTL			GPIO_PCTL_PA3_SSI0FSS
#define   LCD_MOSI_PIN_PCTL   GPIO_PCTL_PA5_SSI0TX

#define   LCD_RST_BASE      GPIOC_BASE
#define   LCD_RST_PIN       PC7


// Screen Buffer
uint8_t screen_buffer[SCREEN_WIDTH_PX * SCREEN_HEIGHT_PG];

//*****************************************************************************
//*****************************************************************************
void init_lcd_gpio(void)
{ 
  gpio_enable_port(LCD_GPIO_BASE);
	gpio_enable_port(LCD_RST_BASE);
  
  // Configure SPI CLK
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CLK_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CLK_PIN_PCTL);
  
  // Configure SPI MOSI
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_MOSI_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_MOSI_PIN_PCTL);
  
  // Configure CS to be a normal GPIO pin that is controlled 
  // explicitly by software
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_alternate_function(LCD_GPIO_BASE, LCD_CS_PIN);
  gpio_config_port_control(LCD_GPIO_BASE, LCD_CS_PIN_PCTL);
	
	// Configure CD to be a normal GPIO pin that is controlled 
  // explicitly by software
  gpio_config_digital_enable(LCD_GPIO_BASE, LCD_CD_PIN);
  gpio_config_enable_output(LCD_GPIO_BASE, LCD_CD_PIN);
	
	// Configure RST_N to be a normal GPIO pin that is controlled 
  // explicitly by software
  gpio_config_digital_enable(LCD_RST_BASE, LCD_RST_PIN);
  gpio_config_enable_output(LCD_RST_BASE, LCD_RST_PIN);
	
  initialize_spi(LCD_SPI_BASE, 3);
}

void draw_buffer(void)
{
	int i, j;
	
	#ifdef RENDER_DEBUG
	printf("Drawing Screen \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	#endif
	
	lcd_clear();
	
	for(i = 0; i < SCREEN_HEIGHT_PG; i++) {
		lcd_set_page(i);
		for(j = 0; j < SCREEN_WIDTH_PX; j++) {
			lcd_set_column(j);
			lcd_write_data(screen_buffer[j + (i * SCREEN_WIDTH_PX)]);
		}
	}
	
}

void draw_buffer_game_board(void)
{
	int i, j, board_start, board_end;
	board_start = ((BOARD_START_X + 1) * TILE_WIDTH_PX);
	board_end = board_start + (BOARD_WIDTH * TILE_WIDTH_PX);
	
	for(i = 0; i < SCREEN_HEIGHT_PG; i++) {
		lcd_set_page(i);
		for(j = board_start; j < board_end; j++) {
			lcd_set_column(j);
			lcd_write_data(screen_buffer[j + (i * SCREEN_WIDTH_PX)]);
		}
	}
	
}

void clear_buffer(void)
{
	int i, j;
	
	for(i = 0; i < SCREEN_HEIGHT_PG; i++) {
		for(j = 0; j < SCREEN_WIDTH_PX; j++) {
			screen_buffer[j + (i * SCREEN_WIDTH_PX)] = 0;
		}
	}
}

void clear_buffer_game_board(void)
{
	int i, j, board_start, board_end;
	board_start = ((BOARD_START_X + 1) * TILE_WIDTH_PX);
	board_end = board_start + (BOARD_WIDTH * TILE_WIDTH_PX);
	
	for(i = 0; i < SCREEN_HEIGHT_PG; i++) {
		for(j = board_start; j < board_end; j++) {
			screen_buffer[j + (i * SCREEN_WIDTH_PX)] = 0;
		}
	}
}

void fill_pixel(int x, int y)
{
	if(UpdateLED) {
		UpdateLED = 0;
		print_led();
	}
	
	#ifdef RENDER_DEBUG
	printf("FILL PIXEL x(%d) y(%d) \n", x, y);
	#endif
	
	if(x >= SCREEN_WIDTH_PX || y >= SCREEN_HEIGHT_PX)
		return;
	
	screen_buffer[x + ((y / PAGE_HEIGHT_PX) * SCREEN_WIDTH_PX)] |= (1 << (y % PAGE_HEIGHT_PX));
}