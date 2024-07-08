#include"StudentManager.h"
#include"UI.h"
#include"Controller.h"

int main()
{

	StudentManager* p_StuManager = StudentManager::GetInstance();
	UI* ui = new UI();
	Controller controller(*ui,*p_StuManager);
	
	controller.start();
	delete ui;

	return 0;
}