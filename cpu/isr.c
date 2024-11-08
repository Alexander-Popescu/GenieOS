#include "isr.h"
#include "../drivers/VGAText.h"
#include "../util/util.h"
#include "idt.h"

isr_t interrupt_handlers[256];

void addInterruptHandler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void populateIDT();

void initISR() {
    //loads IDT with ISR entries and then puts it into the register
    populateIDT(); 
    initIDT();
}

void ISRHandler(registers_t r) {
    printString("received interrupt: ", -1, 0);
    char s[3];
    intToAscii(r.int_no, s);
    printString(s, -1, 0);
    printString("\n", -1, 0);
}

void IRQHandler(registers_t r) {
    // end of interrupt command code, otherwise we wont get another interupt
    if (r.int_no >= 40) wIO8(0xA0, 0x20);
    wIO8(0x20, 0x20);

    if (interrupt_handlers[r.int_no] != 0) {
	isr_t handler = interrupt_handlers[r.int_no];
	handler(r);
    }
}

void populateIDT() {
    //yeah...
    fillIDTEntry(0, (uint32_t)isr0);
    fillIDTEntry(1, (uint32_t)isr1);
    fillIDTEntry(2, (uint32_t)isr2);
    fillIDTEntry(3, (uint32_t)isr3);
    fillIDTEntry(4, (uint32_t)isr4);
    fillIDTEntry(5, (uint32_t)isr5);
    fillIDTEntry(6, (uint32_t)isr6);
    fillIDTEntry(7, (uint32_t)isr7);
    fillIDTEntry(8, (uint32_t)isr8);
    fillIDTEntry(9, (uint32_t)isr9);
    fillIDTEntry(10, (uint32_t)isr10);
    fillIDTEntry(11, (uint32_t)isr11);
    fillIDTEntry(12, (uint32_t)isr12);
    fillIDTEntry(13, (uint32_t)isr13);
    fillIDTEntry(14, (uint32_t)isr14);
    fillIDTEntry(15, (uint32_t)isr15);
    fillIDTEntry(16, (uint32_t)isr16);
    fillIDTEntry(17, (uint32_t)isr17);
    fillIDTEntry(18, (uint32_t)isr18);
    fillIDTEntry(19, (uint32_t)isr19);
    fillIDTEntry(20, (uint32_t)isr20);
    fillIDTEntry(21, (uint32_t)isr21);
    fillIDTEntry(22, (uint32_t)isr22);
    fillIDTEntry(23, (uint32_t)isr23);
    fillIDTEntry(24, (uint32_t)isr24);
    fillIDTEntry(25, (uint32_t)isr25);
    fillIDTEntry(26, (uint32_t)isr26);
    fillIDTEntry(27, (uint32_t)isr27);
    fillIDTEntry(28, (uint32_t)isr28);
    fillIDTEntry(29, (uint32_t)isr29);
    fillIDTEntry(30, (uint32_t)isr30);
    fillIDTEntry(31, (uint32_t)isr31);

	// Remap the PIC
    wIO8(0x20, 0x11);
    wIO8(0xA0, 0x11);
    wIO8(0x21, 0x20);
    wIO8(0xA1, 0x28);
    wIO8(0x21, 0x04);
    wIO8(0xA1, 0x02);
    wIO8(0x21, 0x01);
    wIO8(0xA1, 0x01);
    wIO8(0x21, 0x0);
    wIO8(0xA1, 0x0); 

	
    // interrupts 32 to 47 are mapped to IRQs
    fillIDTEntry(32, (uint32_t)irq0);
    fillIDTEntry(33, (uint32_t)irq1);
    fillIDTEntry(34, (uint32_t)irq2);
    fillIDTEntry(35, (uint32_t)irq3);
    fillIDTEntry(36, (uint32_t)irq4);
    fillIDTEntry(37, (uint32_t)irq5);
    fillIDTEntry(38, (uint32_t)irq6);
    fillIDTEntry(39, (uint32_t)irq7);
    fillIDTEntry(40, (uint32_t)irq8);
    fillIDTEntry(41, (uint32_t)irq9);
    fillIDTEntry(42, (uint32_t)irq10);
    fillIDTEntry(43, (uint32_t)irq11);
    fillIDTEntry(44, (uint32_t)irq12);
    fillIDTEntry(45, (uint32_t)irq13);
    fillIDTEntry(46, (uint32_t)irq14);
    fillIDTEntry(47, (uint32_t)irq15);
    fillIDTEntry(48, (uint32_t)isr48);
    fillIDTEntry(49, (uint32_t)isr49);
    fillIDTEntry(50, (uint32_t)isr50);
    fillIDTEntry(51, (uint32_t)isr51);
    fillIDTEntry(52, (uint32_t)isr52);
    fillIDTEntry(53, (uint32_t)isr53);
    fillIDTEntry(54, (uint32_t)isr54);
    fillIDTEntry(55, (uint32_t)isr55);
    fillIDTEntry(56, (uint32_t)isr56);
    fillIDTEntry(57, (uint32_t)isr57);
    fillIDTEntry(58, (uint32_t)isr58);
    fillIDTEntry(59, (uint32_t)isr59);
    fillIDTEntry(60, (uint32_t)isr60);
    fillIDTEntry(61, (uint32_t)isr61);
    fillIDTEntry(62, (uint32_t)isr62);
    fillIDTEntry(63, (uint32_t)isr63);
    fillIDTEntry(64, (uint32_t)isr64);
    fillIDTEntry(65, (uint32_t)isr65);
    fillIDTEntry(66, (uint32_t)isr66);
    fillIDTEntry(67, (uint32_t)isr67);
    fillIDTEntry(68, (uint32_t)isr68);
    fillIDTEntry(69, (uint32_t)isr69);
    fillIDTEntry(70, (uint32_t)isr70);
    fillIDTEntry(71, (uint32_t)isr71);
    fillIDTEntry(72, (uint32_t)isr72);
    fillIDTEntry(73, (uint32_t)isr73);
    fillIDTEntry(74, (uint32_t)isr74);
    fillIDTEntry(75, (uint32_t)isr75);
    fillIDTEntry(76, (uint32_t)isr76);
    fillIDTEntry(77, (uint32_t)isr77);
    fillIDTEntry(78, (uint32_t)isr78);
    fillIDTEntry(79, (uint32_t)isr79);
    fillIDTEntry(80, (uint32_t)isr80);
    fillIDTEntry(81, (uint32_t)isr81);
    fillIDTEntry(82, (uint32_t)isr82);
    fillIDTEntry(83, (uint32_t)isr83);
    fillIDTEntry(84, (uint32_t)isr84);
    fillIDTEntry(85, (uint32_t)isr85);
    fillIDTEntry(86, (uint32_t)isr86);
    fillIDTEntry(87, (uint32_t)isr87);
    fillIDTEntry(88, (uint32_t)isr88);
    fillIDTEntry(89, (uint32_t)isr89);
    fillIDTEntry(90, (uint32_t)isr90);
    fillIDTEntry(91, (uint32_t)isr91);
    fillIDTEntry(92, (uint32_t)isr92);
    fillIDTEntry(93, (uint32_t)isr93);
    fillIDTEntry(94, (uint32_t)isr94);
    fillIDTEntry(95, (uint32_t)isr95);
    fillIDTEntry(96, (uint32_t)isr96);
    fillIDTEntry(97, (uint32_t)isr97);
    fillIDTEntry(98, (uint32_t)isr98);
    fillIDTEntry(99, (uint32_t)isr99);
    fillIDTEntry(100, (uint32_t)isr100);
    fillIDTEntry(101, (uint32_t)isr101);
    fillIDTEntry(102, (uint32_t)isr102);
    fillIDTEntry(103, (uint32_t)isr103);
    fillIDTEntry(104, (uint32_t)isr104);
    fillIDTEntry(105, (uint32_t)isr105);
    fillIDTEntry(106, (uint32_t)isr106);
    fillIDTEntry(107, (uint32_t)isr107);
    fillIDTEntry(108, (uint32_t)isr108);
    fillIDTEntry(109, (uint32_t)isr109);
    fillIDTEntry(110, (uint32_t)isr110);
    fillIDTEntry(111, (uint32_t)isr111);
    fillIDTEntry(112, (uint32_t)isr112);
    fillIDTEntry(113, (uint32_t)isr113);
    fillIDTEntry(114, (uint32_t)isr114);
    fillIDTEntry(115, (uint32_t)isr115);
    fillIDTEntry(116, (uint32_t)isr116);
    fillIDTEntry(117, (uint32_t)isr117);
    fillIDTEntry(118, (uint32_t)isr118);
    fillIDTEntry(119, (uint32_t)isr119);
    fillIDTEntry(120, (uint32_t)isr120);
    fillIDTEntry(121, (uint32_t)isr121);
    fillIDTEntry(122, (uint32_t)isr122);
    fillIDTEntry(123, (uint32_t)isr123);
    fillIDTEntry(124, (uint32_t)isr124);
    fillIDTEntry(125, (uint32_t)isr125);
    fillIDTEntry(126, (uint32_t)isr126);
    fillIDTEntry(127, (uint32_t)isr127);
    fillIDTEntry(128, (uint32_t)isr128);
    fillIDTEntry(129, (uint32_t)isr129);
    fillIDTEntry(130, (uint32_t)isr130);
    fillIDTEntry(131, (uint32_t)isr131);
    fillIDTEntry(132, (uint32_t)isr132);
    fillIDTEntry(133, (uint32_t)isr133);
    fillIDTEntry(134, (uint32_t)isr134);
    fillIDTEntry(135, (uint32_t)isr135);
    fillIDTEntry(136, (uint32_t)isr136);
    fillIDTEntry(137, (uint32_t)isr137);
    fillIDTEntry(138, (uint32_t)isr138);
    fillIDTEntry(139, (uint32_t)isr139);
    fillIDTEntry(140, (uint32_t)isr140);
    fillIDTEntry(141, (uint32_t)isr141);
    fillIDTEntry(142, (uint32_t)isr142);
    fillIDTEntry(143, (uint32_t)isr143);
    fillIDTEntry(144, (uint32_t)isr144);
    fillIDTEntry(145, (uint32_t)isr145);
    fillIDTEntry(146, (uint32_t)isr146);
    fillIDTEntry(147, (uint32_t)isr147);
    fillIDTEntry(148, (uint32_t)isr148);
    fillIDTEntry(149, (uint32_t)isr149);
    fillIDTEntry(150, (uint32_t)isr150);
    fillIDTEntry(151, (uint32_t)isr151);
    fillIDTEntry(152, (uint32_t)isr152);
    fillIDTEntry(153, (uint32_t)isr153);
    fillIDTEntry(154, (uint32_t)isr154);
    fillIDTEntry(155, (uint32_t)isr155);
    fillIDTEntry(156, (uint32_t)isr156);
    fillIDTEntry(157, (uint32_t)isr157);
    fillIDTEntry(158, (uint32_t)isr158);
    fillIDTEntry(159, (uint32_t)isr159);
    fillIDTEntry(160, (uint32_t)isr160);
    fillIDTEntry(161, (uint32_t)isr161);
    fillIDTEntry(162, (uint32_t)isr162);
    fillIDTEntry(163, (uint32_t)isr163);
    fillIDTEntry(164, (uint32_t)isr164);
    fillIDTEntry(165, (uint32_t)isr165);
    fillIDTEntry(166, (uint32_t)isr166);
    fillIDTEntry(167, (uint32_t)isr167);
    fillIDTEntry(168, (uint32_t)isr168);
    fillIDTEntry(169, (uint32_t)isr169);
    fillIDTEntry(170, (uint32_t)isr170);
    fillIDTEntry(171, (uint32_t)isr171);
    fillIDTEntry(172, (uint32_t)isr172);
    fillIDTEntry(173, (uint32_t)isr173);
    fillIDTEntry(174, (uint32_t)isr174);
    fillIDTEntry(175, (uint32_t)isr175);
    fillIDTEntry(176, (uint32_t)isr176);
    fillIDTEntry(177, (uint32_t)isr177);
    fillIDTEntry(178, (uint32_t)isr178);
    fillIDTEntry(179, (uint32_t)isr179);
    fillIDTEntry(180, (uint32_t)isr180);
    fillIDTEntry(181, (uint32_t)isr181);
    fillIDTEntry(182, (uint32_t)isr182);
    fillIDTEntry(183, (uint32_t)isr183);
    fillIDTEntry(184, (uint32_t)isr184);
    fillIDTEntry(185, (uint32_t)isr185);
    fillIDTEntry(186, (uint32_t)isr186);
    fillIDTEntry(187, (uint32_t)isr187);
    fillIDTEntry(188, (uint32_t)isr188);
    fillIDTEntry(189, (uint32_t)isr189);
    fillIDTEntry(190, (uint32_t)isr190);
    fillIDTEntry(191, (uint32_t)isr191);
    fillIDTEntry(192, (uint32_t)isr192);
    fillIDTEntry(193, (uint32_t)isr193);
    fillIDTEntry(194, (uint32_t)isr194);
    fillIDTEntry(195, (uint32_t)isr195);
    fillIDTEntry(196, (uint32_t)isr196);
    fillIDTEntry(197, (uint32_t)isr197);
    fillIDTEntry(198, (uint32_t)isr198);
    fillIDTEntry(199, (uint32_t)isr199);
    fillIDTEntry(200, (uint32_t)isr200);
    fillIDTEntry(201, (uint32_t)isr201);
    fillIDTEntry(202, (uint32_t)isr202);
    fillIDTEntry(203, (uint32_t)isr203);
    fillIDTEntry(204, (uint32_t)isr204);
    fillIDTEntry(205, (uint32_t)isr205);
    fillIDTEntry(206, (uint32_t)isr206);
    fillIDTEntry(207, (uint32_t)isr207);
    fillIDTEntry(208, (uint32_t)isr208);
    fillIDTEntry(209, (uint32_t)isr209);
    fillIDTEntry(210, (uint32_t)isr210);
    fillIDTEntry(211, (uint32_t)isr211);
    fillIDTEntry(212, (uint32_t)isr212);
    fillIDTEntry(213, (uint32_t)isr213);
    fillIDTEntry(214, (uint32_t)isr214);
    fillIDTEntry(215, (uint32_t)isr215);
    fillIDTEntry(216, (uint32_t)isr216);
    fillIDTEntry(217, (uint32_t)isr217);
    fillIDTEntry(218, (uint32_t)isr218);
    fillIDTEntry(219, (uint32_t)isr219);
    fillIDTEntry(220, (uint32_t)isr220);
    fillIDTEntry(221, (uint32_t)isr221);
    fillIDTEntry(222, (uint32_t)isr222);
    fillIDTEntry(223, (uint32_t)isr223);
    fillIDTEntry(224, (uint32_t)isr224);
    fillIDTEntry(225, (uint32_t)isr225);
    fillIDTEntry(226, (uint32_t)isr226);
    fillIDTEntry(227, (uint32_t)isr227);
    fillIDTEntry(228, (uint32_t)isr228);
    fillIDTEntry(229, (uint32_t)isr229);
    fillIDTEntry(230, (uint32_t)isr230);
    fillIDTEntry(231, (uint32_t)isr231);
    fillIDTEntry(232, (uint32_t)isr232);
    fillIDTEntry(233, (uint32_t)isr233);
    fillIDTEntry(234, (uint32_t)isr234);
    fillIDTEntry(235, (uint32_t)isr235);
    fillIDTEntry(236, (uint32_t)isr236);
    fillIDTEntry(237, (uint32_t)isr237);
    fillIDTEntry(238, (uint32_t)isr238);
    fillIDTEntry(239, (uint32_t)isr239);
    fillIDTEntry(240, (uint32_t)isr240);
    fillIDTEntry(241, (uint32_t)isr241);
    fillIDTEntry(242, (uint32_t)isr242);
    fillIDTEntry(243, (uint32_t)isr243);
    fillIDTEntry(244, (uint32_t)isr244);
    fillIDTEntry(245, (uint32_t)isr245);
    fillIDTEntry(246, (uint32_t)isr246);
    fillIDTEntry(247, (uint32_t)isr247);
    fillIDTEntry(248, (uint32_t)isr248);
    fillIDTEntry(249, (uint32_t)isr249);
    fillIDTEntry(250, (uint32_t)isr250);
    fillIDTEntry(251, (uint32_t)isr251);
    fillIDTEntry(252, (uint32_t)isr252);
    fillIDTEntry(253, (uint32_t)isr253);
    fillIDTEntry(254, (uint32_t)isr254);
    fillIDTEntry(255, (uint32_t)isr255);
}
