
#include "System.h"
#include "sublibinal_config.h"
#include "Timer.h"
#include "packetizer.h"
#include "UART.h"

uint8 tx[256];
extern volatile int timestamp_a, timestamp_b, timestamp_c;
extern volatile uint8 fire_a, fire_b, fire_c;

/*
 * Magic numbers taken from PIC32MX1xx/2xx family datasheet [Table 7-1].
 */

void __attribute__((vector (27))) __attribute__((naked)) comparator_1_isr()
{
    asm volatile("DI");
    asm volatile("SW $s4, T1CONSET");
    asm volatile("LW $t0, TMR1");
    asm volatile("SW $t0, timestamp_a");
    asm volatile("SW $s1, IEC1CLR");
    asm volatile("SW $s1, fire_a");
    asm volatile("ADDI $s0, $s0, -1");
    asm volatile("BNE $s0, $0, #12");
    asm volatile("JAL send_timestamps");
    asm volatile("LI $s0, 3");
    asm volatile("EI");
    asm volatile("ERET");
}

void __attribute__((vector (28))) __attribute__((naked)) comparator_2_isr()
{
    asm volatile("DI");
    asm volatile("SW $s4, T1CONSET");
    asm volatile("LW $t0, TMR1");
    asm volatile("SW $t0, timestamp_b");
    asm volatile("SW $s2, IEC1CLR");
    asm volatile("SW $s1, fire_b");
    asm volatile("ADDI $s0, $s0, -1");
    asm volatile("BNE $s0, $0, #12");
    asm volatile("JAL send_timestamps");
    asm volatile("LI $s0, 3");
    asm volatile("EI");
    asm volatile("ERET");
}

void __attribute__((vector (29))) __attribute__((naked)) comparator_3_isr()
{
    asm volatile("DI");
    asm volatile("SW $s4, T1CONSET");
    asm volatile("LW $t0, TMR1");
    asm volatile("SW $t0, timestamp_c");
    asm volatile("SW $s3, IEC1CLR");
    asm volatile("SW $s1, fire_c");
    asm volatile("ADDI $s0, $s0, -1");
    asm volatile("BNE $s0, $0, #12");
    asm volatile("JAL send_timestamps");
    asm volatile("LI $s0, 3");
    asm volatile("EI");
    asm volatile("ERET");
}

/**
 * Send the recorded timestamps up the UART.
 *
 * @note This function will automatically save context before entering, so it
 *       is okay for it to be written in C.
 *
 * @return None.
 */
void send_timestamps()
{
    uint8 message[9];

    /*
     * Disable the timer and reset flags.
     */
    disable_Timer(Timer_1);
    TMR1 = 0;

    message[0] = timestamp_a >> 8 & 0xFF;
    message[1] = timestamp_a & 0xFF;
    message[2] = timestamp_b >> 8 & 0xFF;
    message[3] = timestamp_b & 0xFF;
    message[4] = timestamp_c >> 8 & 0xFF;
    message[5] = timestamp_c & 0xFF;
    message[6] = fire_a;
    message[7] = fire_b;
    message[8] = fire_c;

    /*
     * Set all fired flags back to 0.
     */
    fire_a = fire_b = fire_c = 0;

    send_packet(PACKET_UART_CH_1, message, 9);
}

/**
 * Timer interrupt callback function. [ISR]
 *
 * @note If this function is called, it means that a hydrophone did not trigger.
 *       We need to send up all available comparator stamps and indicate a
 *       missed read.
 */
void timer_overflow()
{
    send_timestamps();
}

/**
 * Setup the timer to use for hydrophone timing.
 *
 * @note This function should ensure that the clock divider on the timer is as
 *       low as possible to maximize the accuracy of readings. Additionally,
 *       The period should also be at the maximum to ensure the timer does
 *       not overflow.
 *
 * @return None.
 */
void setup_timer()
{
    Timer_Config timer = {0};
    timer.which_timer = Timer_1;
    timer.frequency = 5;
    timer.pbclk = PB_CLK;
    timer.callback = &timer_overflow;
    timer.enabled = FALSE;

    initialize_Timer(timer);

    /*
     * Update the period and divider registers.
     */
    update_period_Timer(Timer_1, 65535);
    update_divider_Timer(Timer_1, Div_1);
}

/**
 * Setup the UART for transmitting data up to the computer when it is available.
 *
 * @note The UART should operate at 115200 baud rate and should operate with
 *       packetized communications.
 *
 * @return None.
 */
void setup_uart()
{
    UART_Config uart = {0};
    uart.speed = 115200;
    uart.pb_clk = PB_CLK;
    uart.which_uart = UART_CH_1;
    uart.tx_buffer_ptr = tx;
    uart.tx_buffer_size = sizeof(tx);
    uart.tx_en = TRUE;
    uart.tx_pin = Pin_RPB4;

    Packetizer_Config p = {0};
    p.which_channel = PACKET_UART_CH_1;
    p.control_byte = '!';
    p.uart_config = uart;

    initialize_packetizer(p);
}

/**
 * Setup 3 comparators to interrupt on change from reference.
 *
 * @note The comparators will use an external voltage reference on their respect
 *       reference inputs. They should interrupt on a low->high transition of
 *       comparator output.
 *
 * @return None.
 */
void setup_comparator()
{
    /*
     * Setup input pin C1IND (B0), C1INB (B2), and C3INB (B14).
     */
    //CMP1 in
    ANSELBbits.ANSB0 = 1;
    TRISBbits.TRISB0 = 1;
    //CMP2 in
    ANSELBbits.ANSB2 = 1;
    TRISBbits.TRISB2 = 1;
    //CMP3 in
    ANSELBbits.ANSB14 = 1;
    TRISBbits.TRISB14 = 1;

    /*
     * Setup the negative input terminals on B3, B1, and B15.
     */
    //CMP1 ref
    TRISBbits.TRISB3 = 1;
    ANSELBbits.ANSB3 = 1;
    //CMP2 ref
    TRISBbits.TRISB1 = 1;
    ANSELBbits.ANSB1 = 1;
    //CMP3 ref
    TRISBbits.TRISB15 = 1;
    ANSELBbits.ANSB15 = 1;

    /*
     * Setup comparator interrupts.
     */
    //CMP1
    IFS1bits.CMP1IF = 0;
    IPC6bits.CMP1IP = 7;
    IEC1bits.CMP1IE = 1;
    //CMP2
    IFS1bits.CMP2IF = 0;
    IPC7bits.CMP2IP = 7;
    IEC1bits.CMP2IE = 1;
    //CMP3
    IFS1bits.CMP3IF = 0;
    IPC7bits.CMP3IP = 7;
    IEC1bits.CMP3IE = 1;

    /*
     * Turn comparator one on for low-> high transition on C1IND using
     * external voltage reference.
     */
    CM1CON = 1<<15 | 1<<6 | 1<<1;

    /*
     * Turn comparator two on for low-> high transition on C2IND using
     * external voltage reference.
     */
    CM2CON = 1<<15 | 1<<6 | 1<<1;

    /*
     * Turn comparator three on for low-> high transition on C1INB using
     * external voltage reference.
     */
    CM3CON = 1<<15 | 1<<6;
}

/**
 * The main entry point to the program.
 *
 * @return 0.
 */
int main()
{
    /*
     * Set up a timer with a divider of 1. A single 16-bit timer will be
     * sufficient because the speed of sound in water is 1482m/s.
     * A single timer therefore will not overflow until sound has travelled
     * roughly 6.5 meters in water. Our hydrophones are much closer than this.
     * If we notice that the timer overflows, we can trigger an error and indicate
     * that not all of our hydrophones triggered and send up partial results.
     */
    setup_timer();

    /*
     * Set up the UART with 115.2k baud for transmitting results to the computer.
     */
    setup_uart();

    /*
     * Set up and enable interrupts for the analog comparator module to trigger
     * on hydrophone reads.
     */
    setup_comparator();

    /*
     * Enable interrupts in multi-vectored mode.
     */
    enable_Interrupts();

    /*
     * From this point on in execution, all operations will occur ENTIRELY out
     * of registers. The only functions that should ever be called will be ISRs
     * for the timer, UART transmission, and ISRs for comparators.
     *
     * Register mapping reference:
     * s0: Number of comparators not triggered. Upon reaching 0, a UART packet
     *     is sent.
     * s1: CMP1IFG mask. To be used for clearing the CMP1 IFG.
     * s2: CMP2IFG mask. To be used for clearing the CMP2 IFG.
     * s3: CMP3IFG mask. To be used for clearing the CMP3 IFG.
     * s4: Timer enable mask. Used for enabling the timer.
     */
    asm volatile("LW $s0, 3");
    asm volatile("LW $s1, 1<<0");
    asm volatile("LW $s2, 1<<1");
    asm volatile("LW $s3, 1<<2");
    asm volatile("LW $s4, 1<<15");
    while (1);
}
