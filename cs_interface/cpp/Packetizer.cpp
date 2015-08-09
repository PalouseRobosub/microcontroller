#include "Packetizer.hpp"

    Packetizer::Packetizer(const char *port_name, uint8_t control_byte)
    {
        this->control_byte = control_byte;
        serial = new Serial(port_name);
    }

    Packetizer::~Packetizer()
    {
        delete serial;
    }

    int Packetizer::get(uint8_t *buf)
    {
        uint8_t header[2];
        bool success = false;
        int size = -1;

        while (!success)
        {
            //get the packet header
            serial->sread(header, 2);

            //check for control byte
            if(header[0] != this->control_byte)
            {
                //we don't have sync, aquire sync
                this->get_lock();
                continue;
            }//else, we are in sync, read the data in the packet


            size = header[1]; //extract the packet size
            serial->sread(buf, header[1]); //read the data into the buffer
            success = true;
        }

        return size;
    }

    int Packetizer::send(uint8_t *buf, uint8_t num)
    {
        uint8_t send_buf[MAX_PACKET_SIZE+2];

        //fill out the header info
        send_buf[0] = this->control_byte;
        send_buf[1] = num;

        //place data into the transmit buffer
        memcpy(send_buf+2, buf, num);

        //send the packet
        serial->swrite(send_buf, num+2);

        return 0;
    }

    void Packetizer::get_lock(void)
    {
        uint8_t buf[MAX_PACKET_SIZE+2];
        uint8_t tmp_byte;
        bool in_sync = false;

        printf("Aquiring stream sync");
        while(!in_sync)
        {
            //get a single byte
            serial->sread(&tmp_byte, 1);

            //if current byte is control byte,
            //assume we're in sync
            if(tmp_byte == this->control_byte)
            {
                //read what should be the size byte
                serial->sread(&tmp_byte, 1);

                //read the rest of the packet(length of size byte),
                //plus two more bytes (should be next packet's control and size bytes)
                serial->sread(buf, tmp_byte+2);

                //check again for new start byte. If it's right,
                //then we can say with a high degree of certainty we
                //are in sync
                if(buf[tmp_byte] == this->control_byte)
                {
                    in_sync = true;
                    //read and throw away the rest of the packet
                    serial->sread(buf, buf[tmp_byte+1]);
                }
            }
        }
        printf("Sync locked!");
    }
