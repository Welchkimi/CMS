#include "cmscontroller.h"
#include "cms.h"
#include "operators.h"
#include "admins.h"
#include "assetsunitmanagercontroller.h"
#include "assetsunitmanager.h"
#include "operatorscontroller.h"


CmsController::CmsController(const CmsController &)
    : ApplicationController()
{ }

void CmsController::index()
{
    QList<Cms> cmsList = Cms::getAll();
    texport(cmsList);
    render();
}

void CmsController::show_operator(const QString &pk)
{

    //auto cms = Cms::get(pk);
    auto operators = Operators::get(pk);
    QString adminID = session().value("adminID").toString();
    if(operators.isNull()){
        QString error = "该操作员不存在!";
        texport(error);
        texport(adminID);
        texport(operators);
        render();
    }else{
        texport(adminID);
        texport(operators);
        render();
    }
}

void CmsController::show_manager(const QString &pk)
{
    auto assetsunitmanager = Assetsunitmanager::get(pk.toInt());
    //QString adminID = session().value("adminID").toString();
    if(assetsunitmanager.isNull()){
        QString error = "该资产管理人不存在!";
        texport(error);
        //texport(adminID);
        texport(assetsunitmanager);
        render();
    }else{
       // texport(adminID);
        texport(assetsunitmanager);
        render();
    }

}

void CmsController::entry()
{
    renderEntry();
}

void CmsController::newmanager()
{
    render("newmanager");
}

void CmsController::createmanager()
{
    if (httpRequest().method() != Tf::Post) {
        return;
    }

   auto form = httpRequest().formItems("assetsunitmanager");
   QString ID = form["managerID"].toString();
   auto assetsunitmanager = Assetsunitmanager::create(form);
   if(!assetsunitmanager.isNull() && Assetsunitmanager::checkmanagerID(ID)){
        redirect(urla("list_manager"));

   }else{
       QString error = "资产管理人编号已经存在!";
       texport(error);
       renderEntry2(form);
   }

}

void CmsController::create()
{
    if (httpRequest().method() != Tf::Post) {
        return;
    }

    auto form = httpRequest().formItems("operators");
    QString ID = form["operatorID"].toString();
    auto operators = Operators::create(form);
    Cms cms;
    if (!operators.isNull() && cms.checkoperatorID(ID)) {
        QString adminID = session().value("adminID").toString();
        QString notice  = form["operatorID"].toString();

       if(cms.insert_connection(adminID,notice)){
           // QString notice = "Created successfully.";

      //  QString notice = b;
     //  texport(opr);
         //texport(notice);
       // tflash(notice);
            redirect(urla("list_operator", adminID));
          // render("index");
        // redirect(urla("index"));
       }
    } else {
        QString error = "操作员编号已经存在!";
        texport(error);
        renderEntry(form);
    }
}

void CmsController::admin_login()
{
    render("admin_login");
}

void CmsController::adminlogin()
{
   // renderEntry();
    if(httpRequest().method() != Tf::Post){
        return;
    }
    auto form = httpRequest().formItems("cms");
    Cms cms;
    if(cms.adminlogin(form)){
       // QString notice = "Welcom "+form["numberID"].toString()+"";
        //tflash(notice);
        //session().insert("adminID",form["numberID"].toString());
       // redirect(urla("admin_center",form["numberID"].toString()))
       // Admins a;
       // a.setAdminID(form["numberID"].toString());
      // printf( a.adminID());
        b = form["numberID"].toString();
        session().insert("adminID",b);
        redirect(urla("admin_center",b));

        //renderText(b);
    }else{
        QString error = "NumberID or Password Error";
        tflash(error);
        redirect(urla("admin_login"));
    }
}

void CmsController::admin_center(const QString &adminID)
{
    texport(adminID);
    render("admin_center"); //在该页面能够正常显示adminID
}

void CmsController::operator_login()
{
    render("operator_login");
}

void CmsController::operatorlogin()
{
   //renderEntry();
    if(httpRequest().method() != Tf::Post){
        return;
    }
    auto form = httpRequest().formItems("cms");
    Cms cms;
    if(cms.operatorlogin(form)){
       // QString notice = "Welcome "+form["numberID"].toString()+"";
       // tflash(notice);
        a = form["numberID"].toString();
        session().insert("operatorID",a);
        redirect(url("assetsunit","index"));
    }else{
        QString error = "NumberID or Password Error";
        tflash(error);
        redirect(urla("operator_login"));
    }
}

void CmsController::operator_center(const QString &operatorID)
{
    texport(operatorID);
    render("operator_center");
}

void CmsController::renderEntry(const QVariantMap &cms)
{
    QString adminID = session().value("adminID").toString();
    texport(cms);
    texport(adminID);
    render("entry");
}

void CmsController::renderEntry2(const QVariantMap &assetsunitmanager)
{
//    QString adminID = session().value("adminID").toString();
//    texport(cms);
//    texport(adminID);
//    render("entry");
    texport(assetsunitmanager);
    render("newmanager");
}

void CmsController::list_operator(const QString &adminID)
{
    //QList<Operators> list_operator();
    Operators form;
    QList<Operators> operator_list = form.list_operator(adminID);
    texport(operator_list);
    //QString adminID = session().value("adminID").toString();
    texport(adminID);
    render("list_operator");

}

void CmsController::search_operator()
{
    if(httpRequest().method() != Tf::Post){
        return;
    }
    auto form = httpRequest().formItems("operators");
    QString operatorID = form["operatorID"].toString();
    redirect(urla("show_operator", operatorID));


}

void CmsController::search_manager()
{
    if(httpRequest().method() != Tf::Post){
        return;
    }
    auto form = httpRequest().formItems("assetsunitmanager");
    QString managerID = form["managerID"].toString();
    redirect(urla("show_manager", managerID));

    //QString adminID = session().value("adminID").toString();
//    Operators a;
//    QList<Operators> operator_list = a.search_operator(operatorID,adminID);
//    if(operator_list.length() == 0){
//        QString error = "该操作员不存在";
//        texport(error);
//        texport(adminID);
//        render("show");
//    }else{
//    texport(operator_list);
//    texport(adminID);
//    render("show");
//    }
}

void CmsController::list_manager()
{
    QString adminID = session().value("adminID").toString();
     Assetsunitmanager form;
     QList<Assetsunitmanager> manager_list = form.list_manager();
     texport(adminID);
     texport(manager_list);
     render("list_manager");

}

void CmsController::edit(const QString &pk)
{
    auto operators = Operators::get(pk);
    if (!operators.isNull()) {
        renderEdit(operators.toVariantMap());
    } else {
        redirect(urla("entry"));
    }
}

void CmsController::changemanager(const QString &managerID)
{
    auto assetsunitmanager = Assetsunitmanager::get(managerID.toInt());
    if(!assetsunitmanager.isNull()){
       renderEdit2(assetsunitmanager.toVariantMap());
    }else{
        redirect(urla("change"));
    }
}
void CmsController::change()
{
     //renderChange();
   // AssetsunitmanagerController a;
   // a.renderEntry();
    renderEntry();
}



void CmsController::save(const QString &pk)
{
    QString adminID = session().value("adminID").toString();
    if (httpRequest().method() != Tf::Post) {
        return;
    }

    QString error;
    auto cms = Operators::get(pk);
    if (cms.isNull()) {
        error = "Original data not found. It may have been updated/removed by another transaction.";
        tflash(error);
        redirect(urla("edit", pk));
        return;
    }

    auto form = httpRequest().formItems("operators");
    cms.setProperties(form);
    if (cms.save()) {
       // QString notice = "Updated successfully.";
        //tflash(notice);
        redirect(urla("list_operator", adminID));
    } else {
        error = "Failed to update.";
        texport(error);
        renderEdit(form);
    }
}

void CmsController::savemanager(const QString &pk)
{
   // QString adminID = session().value("adminID").toString();
    if (httpRequest().method() != Tf::Post) {
        return;
    }

    QString error;
    auto assetsunitmanager = Assetsunitmanager::get(pk.toInt());
    if (assetsunitmanager.isNull()) {
        error = "Original data not found. It may have been updated/removed by another transaction.";
        tflash(error);
        redirect(urla("change", pk));
        return;
    }

    auto form = httpRequest().formItems("assetsunitmanager");
    assetsunitmanager.setProperties(form);
    if (assetsunitmanager.save()) {
       // QString notice = "Updated successfully.";
        //tflash(notice);
        redirect(urla("list_manager"));
    } else {
        error = "Failed to update.";
        texport(error);
        renderEdit(form);
    }

}

void CmsController::renderEdit(const QVariantMap &operators)
{
    QString adminID = session().value("adminID").toString();
    texport(operators);
    texport(adminID);
    render("edit");
}

void CmsController::renderEdit2(const QVariantMap &assetsunitmanager)
{
    texport(assetsunitmanager);
    render("change");
}

void CmsController::remove(const QString &pk)
{
    QString adminID = session().value("adminID").toString();
    if (httpRequest().method() != Tf::Post) {
        return;
    }
    Cms cms;
    if(cms.remove_connection(adminID,pk)){
        redirect(urla("list_operator",adminID));
    }
    //auto cms = Cms::get(pk);
    //cms.remove();
    //redirect(urla("index"));
}

void CmsController::delete_operator(const QString &pk)
{
    QString adminID = session().value("adminID").toString();
    if (httpRequest().method() != Tf::Post) {
        return;
    }
    Cms cms;
    if(cms.delete_operator(pk)){
        redirect(urla("list_operator",adminID));
    }
}

void CmsController::change_status(const QString &operatorID)
{
    QString adminID = session().value("adminID").toString();
    Cms cms;
    if(cms.change_status(operatorID)){
        redirect(urla("list_operator",adminID));
    }
}

void CmsController::cgstatus(const QString &managerID)
{
    Cms cms;;
    if(cms.cgstatus(managerID)){
        redirect(urla("list_manager"));
    }
}

void CmsController::refresh()
{
    QString adminID = session().value("adminID").toString();
    redirect(urla("list_operator",adminID));
}


// Don't remove below this line
T_REGISTER_CONTROLLER(cmscontroller)
