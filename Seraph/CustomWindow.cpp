#include "stdafx.h"
#include "CustomWindow.h"


CustomWindow::CustomWindow() {
}

void CustomWindow::setAkey( Akey* akey ) noexcept {this->akey = akey;}
void CustomWindow::setSetting( Setting* setting ) noexcept {this->setting = setting;}
void CustomWindow::setUserCredential( UserCredential* user_credential ) noexcept {this->user_credential = user_credential;}
Akey* CustomWindow::getAkey() const noexcept {return akey;}
Setting* CustomWindow::getSetting() const noexcept {return setting;}
UserCredential* CustomWindow::getUserCredential() const noexcept {return user_credential;}


CustomWindow::~CustomWindow() {
}
