#include "UI_Element.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2SString.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "UI_Manager.h"
#include "j1Audio.h"

#include "j1Window.h"

UI_Element::UI_Element()
{
}

UI_Element::UI_Element(int x, int y, UI_Element* parent) : Local_pos(x,y), parent(parent)
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;


	config = App->LoadConfig(config_file);
	UI_node = config.child("UI");

	
	//I THINK AUDIOS SHOULDN'T BE LOADED HERE, BUT I'M NOT SURE :/ 
	FXON = UI_node.child("FXON").text().as_string();
	FXPRESSED = UI_node.child("FXPRESSED").text().as_string();
	fxOn = App->audio->LoadFx(FXON.GetString());
	fxPressed = App->audio->LoadFx(FXPRESSED.GetString());
	//IMPORTANT

	if (parent == nullptr) {
		Scree_pos.x = x + -App->render->camera.x;
		Scree_pos.y = y + -App->render->camera.y;
	}
	else {
		Scree_pos.x = parent->Scree_pos.x + Local_pos.x;
		Scree_pos.y = parent->Scree_pos.y + Local_pos.y;
		parent->Son.PushBack(this);
	}
	
}

UI_Element::~UI_Element()
{
}

bool UI_Element::Update(float dt)
{
	


	return true;
}

bool UI_Element::CleanUp()
{

	return true;
}

void UI_Element::Draw(float dt, SDL_Texture* texture)
{
	

	if (parent == nullptr && this->type != PLAYERUIIMAGE && this->type != PLAYERUILABEL) {
		Scree_pos.x = Local_pos.x;
		Scree_pos.y = Local_pos.y;
		
		
	}
	else if (this->type != PLAYERUIIMAGE && this->type != PLAYERUILABEL){
		SetPos(parent->Scree_pos.x + Local_pos.x, parent->Scree_pos.y + Local_pos.y);
	}
	if (WantToRender) {
		if (type != LABEL && type != PLAYERUILABEL)
			App->render->Blit(texture, Scree_pos.x, Scree_pos.y, &(png_pos));
		else {
			App->render->Blit(tex, Scree_pos.x, Scree_pos.y, NULL);
		}

		if (App->ui_manager->debug_draw) {
			App->render->DrawQuad({ Scree_pos.x,Scree_pos.y,width,height }, 255, 0, 0, 255, false);
		}
	}
	
}
bool UI_Element::IsMouseOn()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	if (mouse_pos.x >= Scree_pos.x && Scree_pos.x + width >= mouse_pos.x && mouse_pos.y >= Scree_pos.y && Scree_pos.y + height >= mouse_pos.y) {
		return true;
	}
	return false;
}

void UI_Element::SetPos(int x, int y)
{
	if (CanBeMoved && type != SLIDER) {
		Scree_pos.x = x + -App->render->camera.x;
		Scree_pos.y = y + -App->render->camera.y;
	}
	else Scree_pos.y = y + -App->render->camera.y;
	
}

void UI_Element::SetSpritesData(SDL_Rect Idle, SDL_Rect Hover, SDL_Rect Pressed)
{
	if (type != IMAGE && type != PLAYERUIIMAGE) {
		NoPressedNoMouseOn = Idle;
		MouseOn = Hover;
		this->Pressed = Pressed;
		width = Hover.w;
		height = Hover.h;
	}
	else  {
		png_pos = Idle;
		width = Idle.w;
		height = Idle.h;
	}

}




