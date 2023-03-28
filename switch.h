#ifndef SWTICH_H
#define SWITCH_H
class Switch {
public:
    int x;
    int y;
    int size;
	int r;
	int g;
	int b;
	bool onon;
	Switch();
    Switch(int a, int b, int s);
	void On();
	void drawSwitch();
	void clicked(float gX, float gY, int leader_radius);
};
#endif
