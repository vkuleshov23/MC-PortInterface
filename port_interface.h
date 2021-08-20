/*!
\mainpage 5 bit Port Interface
port_interface.h
*/
/*!
 * \file Файл класса Port Interface, для работы с 5-ти битным портом микроконтроллера
 * \brief Класс для работы с 5-ти битным портом микроконтроллера
*/
#ifndef _5_BIT_PORT_
#define _5_BIT_PORT_
#include <cstdint>
class PortInterface {
private:
	uint8_t DDR = 0b00000000; ///< Регистр управления параметрами ввода/вывода порта
	uint8_t PORT = 0b00000000; ///< Регистр управления состоянием выходов порта
	uint8_t PIN = 0b00000000; ///< Регистр для чтения при состоянии входа порта 
	static const uint8_t DDR_SIZE = 8; ///< размер регистра DDR
	static const uint8_t PORT_WIDTH = 5; ///< ширина порта доступного для использования
	static const uint8_t PORT_WIDTH_BITMASK = 0b00011111; ///< битовая маска для записи только доступных пинов порта 
	/*!
	 * Проверка пина на корректность относительно текущего порта
	 * \param[in] pin Пин порта
	 * \return Корректность порта
	*/ 
	inline bool checkPin(int pin){
		pin = (uint8_t)pin;
		return (pin >= 0 && pin < PORT_WIDTH) ? true : false;
	}
public:
	static const bool ON = true; ///< Значение для переключение пина в состояние 1
	static const bool OFF = false; ///< Значение для переключение пина в состояние 0
	PortInterface() {};
	/*!
	 * Переключение всего порта в состояние вывода (write - 1)
	*/ 
	void setPortOutput();
	/*!
	 * Переключение всего порта в состояние ввода (read - 0)
	*/
	void setPortInput();
	/*!
	 * Переключение одного пина в состояние вывода (write - 1)
	 * \param[in] pin Пин порта
	 * \return Корректность перевода состояния
	*/
	bool setPinOutput(int pin);
	/*!
	 * Переключение одного пина в состояние ввода (read - 0)
	 * \param[in] pin Пин порта
	 * \return Корректность перевода состояния
	*/
	bool setPinInput(int pin);
	/*!
	 * Запись в весь порт (5 bit)
	 * \param[in] data Передаваемая информация
	*/
	void writePort(int data);
	/*!
	 * Чтение из всего порта (5 bit)
	 * \return Целочисленное беззнаковое число - состояние порта
	*/
	int readPort();
	/*!
	 * Запись в один пин порта
	 * \param[in] pin Пин порта
	 * \param[in] data Передаваемая информация
	*/
	void write(int pin, bool data);
	/*!
	 * Чтение из одного пина порта
	 * \param[in] pin Пин порта
	 * \return Состояние пина порта
	*/
	bool read(int pin);
	/*!
	 * Запись в один пин порта
	 * \param[in] pin Пин порта
	 * \param[in] data Передаваемая информация
	*/
	void writeInt(int pin, int data);
	/*!
	 * Чтение из одного пина порта
	 * \param[in] pin Пин порта
	 * \return Состояние пина порта
	*/
	int readInt(int pin);
	~PortInterface() {};
};
#endif

void PortInterface::setPortOutput(){
	DDR |= PORT_WIDTH_BITMASK;
}
void PortInterface::setPortInput(){
	DDR &= ~PORT_WIDTH_BITMASK;
}
bool PortInterface::setPinOutput(int pin){
	if(checkPin(pin)){
		DDR |= 1 << pin;
		return true;
	}
	else{
		exit(-1);
	}
}
bool PortInterface::setPinInput(int pin){
	if(checkPin(pin)){
		DDR &= ~(1 << pin);
		return true;
	}
	else{
		exit(-1);
	}
}
void PortInterface::writePort(int data){
	this->setPortOutput();
	PORT = (PORT & ~PORT_WIDTH_BITMASK) + ((uint8_t)data & PORT_WIDTH_BITMASK);
}
int PortInterface::readPort(){
	this->setPortInput();
	return int(PIN & PORT_WIDTH_BITMASK);
}
void PortInterface::write(int pin, bool data){
	this->writeInt(pin, data ? 1 : 0);
}
bool PortInterface::read(int pin){
	return this->readInt(pin) ? 1 : 0;
}
void PortInterface::writeInt(int pin, int data){
	if(this->setPinOutput(pin)){
		if (data == 0){
			PORT &= ~(1 << pin);
		} else if (data == 1){
			PORT |= (1 << pin);
		} else {
			exit(-1);
		}
	}
}
int PortInterface::readInt(int pin){
	if(this->setPinInput(pin)){
		return (int)(PIN & (1 << pin)) << (DDR_SIZE - pin) >> DDR_SIZE; 
	}
	exit(-1);
}