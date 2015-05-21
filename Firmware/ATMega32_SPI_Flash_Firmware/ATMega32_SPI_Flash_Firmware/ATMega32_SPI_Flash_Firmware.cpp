#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000
#include <util/delay.h>

class SPI_M45PE16{
	// SPI CODE TAKEN FROM: http://embeddlinux.blogspot.co.uk/2014/05/interfacing-m45pe16-spi-flash-to.html
	
	#define WREN 0X06
	#define READ 0X03
	#define PP 0X02

	#define PAGE_SIZE 256
	#define NUM_OF_SECTORS 32
	#define NUM_OF_PAGES_IN_SECTOR 256

	#define FLASH_CS_HIGH() (PORTB|=(1<<4))
	#define FLASH_CS_LOW() (PORTB &=~(1<<4))
	#define WAIT_TRANSFER() while(!(SPSR & (1<<SPIF)))
	
	public:
		SPI_M45PE16(){}
			
		void spi_init(){
			DDRB |= (1<<4) | (1<<5) | (1<<7);
			SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
			SPSR = 0x00;
		}
		
		void spi_write(uint8_t data) {
			FLASH_CS_LOW();
			SPDR = data;
			WAIT_TRANSFER();
			FLASH_CS_HIGH();
		}

		void spi_page_write(uint32_t page_address,uint8_t * data,uint8_t length){
			spi_write(WREN);
			//_delay_ms(10);
			
			FLASH_CS_LOW();
			
			SPDR = PP; WAIT_TRANSFER();
			SPDR = (uint8_t) page_address; WAIT_TRANSFER();
			SPDR = (uint8_t) (page_address>>8); WAIT_TRANSFER();
			SPDR = (uint8_t) (page_address>>16); WAIT_TRANSFER();
			
			for(int i=0;i<length;i++){
				SPDR = data[i];
				WAIT_TRANSFER();
			}
			
			FLASH_CS_HIGH();
		}

		void spi_page_read(uint32_t page_address,uint8_t * buffer,uint8_t length){
			spi_write(WREN);
			//_delay_ms(10);
			
			FLASH_CS_LOW();
			
			SPDR = READ; WAIT_TRANSFER();
			
			SPDR = (uint8_t) page_address;			WAIT_TRANSFER();
			SPDR = (uint8_t) (page_address>8);		WAIT_TRANSFER();
			SPDR = (uint8_t) (page_address>>16);	WAIT_TRANSFER();
			
			for(int i=0;i<length;i++){
				SPDR = 0xF0; WAIT_TRANSFER();
				buffer[i] = SPDR;
			}
			
			FLASH_CS_HIGH();
		}
	private:	
	
};

int main(void) {
	SPI_M45PE16 spi;

	spi.spi_init(); // INIT SPI
	
	uint8_t buff[PAGE_SIZE]; // Page buffer
	
	// Read all sectors:
	for(int page=0;page<NUM_OF_SECTORS;page++){
		spi.spi_page_read(0,buff,PAGE_SIZE-1);
		// And deal with each of them here:
		
	}
	
    while(1){}
}