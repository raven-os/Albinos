# include "Subscription.hpp"

Albinos::Subscription::Subscription(std::string const &associatedSetting, FCPTR_ON_CHANGE_NOTIFIER callBack, void *associatedData)
  : associatedSetting(associatedSetting)
  , callBack(callBack)
  , associatedData(associatedData)
{}

///
/// \todo implementation
///
Albinos::Subscription::~Subscription()
{

}

void Albinos::Subscription::executeCallBack(char const *newValue) const
{
  callBack(associatedData, newValue);
}

std::string const &Albinos::Subscription::getAssociatedSetting() const
{
  return associatedSetting;
}
