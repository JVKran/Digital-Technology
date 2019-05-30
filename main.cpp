#include "hwlib.hpp"

class sensor_pin : public hwlib::pin_in{
	private:
		hwlib::pin_in & sensor;
	public:
		sensor_pin(hwlib::pin_in & sensor): sensor(sensor){}
		bool read() override{
			sensor.refresh();
			hwlib::wait_ms(50);
			return sensor.read();
		}
};

int main( void ){
     	namespace target = hwlib::target; 
    
     	auto scl = target::pin_oc( target::pins::d6 );
     	auto sda = target::pin_oc( target::pins::d7 );
	auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
   
     	auto pcf = hwlib::pcf8574(i2c_bus, 0x38);
	
	auto D1 = pcf.p0;
	auto D2 = pcf.p1;	
	auto CLK1 = pcf.p2;
	auto CLK2 = pcf.p3;

	auto tilt_pin = target::pin_in( target::pins::d28 );
	auto radar_pin = target::pin_in( target::pins::d33 );

	auto tilt = sensor_pin(tilt_pin);
	auto radar = sensor_pin(radar_pin);

	for(;;){
		D1.write(tilt.read());
		D1.flush();
		D2.write(radar.read());
		D2.flush();
		hwlib::wait_ms(20);
		CLK1.write(1);
		CLK1.flush();
		CLK2.write(1);
		CLK2.flush();
		hwlib::wait_ms(20);
		CLK1.write(0);
		CLK1.flush();
		CLK2.write(0);
		CLK2.flush();
		hwlib::cout << tilt.read() << " en " << radar.read() << hwlib::endl;
		hwlib::wait_ms(1000);
	}

}
