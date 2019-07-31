#include "KeyboardSp.h"

#include <QDebug>
#include <QGuiApplication>

#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QtAndroidExtras>
#endif

#if defined(Q_OS_IOS)

#import <UIKit/UIKit.h>

static UIViewController *qtController = nil;

@interface KeyboardNotificationIos : NSObject {}

- (id) initNotifications;
- (void) keyboardWillShow:(NSNotification *)note;
- (void) keyboardDidShow:(NSNotification *)note;
- (void) keyboardWillHide:(NSNotification *)note ;
- (void) keyboardDidHide:(NSNotification *)note;

@end

@implementation KeyboardNotificationIos

- (id) initNotifications {
    [super init];

    // При появлении клавиатуры вызывается селектор keyboardWillShow:
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(keyboardWillShow:)
                                          name:@"UIKeyboardWillShowNotification"
                                          object:nil];
    // Клавиатура польностью показана
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(keyboardDidShow:)
                                          name:@"UIKeyboardDidShowNotification"
                                          object:nil];
      // При скрытии клавиатуры
    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(keyboardWillHide:)
                                          name:@"UIKeyboardWillHideNotification"
                                          object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                          selector:@selector(keyboardDidHide:)
                                          name:@"UIKeyboardDidHideNotification"
                                          object:nil];
}

-(void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    //???? TODO - посмотреть зачем
    [self release];
    [super dealloc];
}

- (void) keyboardWillShow:(NSNotification *)note {
    NSDictionary *userInfo = [note userInfo];
    CGSize kbSize = [[userInfo objectForKey:UIKeyboardFrameBeginUserInfoKey] CGRectValue].size;
//    CGSize sizeOfView = self.view.bounds.size;
    //NSLog(@"keyboardWillShow");// in Obj - C code height = %f,height = %f", kbSize.height,sizeOfView.height);
//    int result = (int)roundf(sizeOfView.height - kbSize.height - 20 );

    //DEBUG!!!
    if (qtController == nil) {
        UIWindow *window  = [UIApplication sharedApplication].keyWindow;
        qtController = [window rootViewController];
    }

    int result = (int)roundf(qtController.view.bounds.size.height - kbSize.height);
    sp::KeyboardSp &instance = sp::KeyboardSp::instance();
    instance.sendVisibleChanged(true, result);
}

- (void) keyboardDidShow:(NSNotification *)note {
    #pragma unused(note)
}

- (void) keyboardWillHide:(NSNotification *)note {
    #pragma unused(note)
}

- (void) keyboardDidHide:(NSNotification *)note {
    #pragma unused(note)
    int result = (int)roundf(qtController.view.bounds.size.height);
    sp::KeyboardSp &instance = sp::KeyboardSp::instance();
    instance.sendVisibleChanged(false, result);
}
@ end
#endif

//------------------------------------------------------------------------------
sp::KeyboardSp &sp::KeyboardSp::instance()
{
    static KeyboardSp instance;
    return instance;
}

//------------------------------------------------------------------------------
void sp::KeyboardSp::sendVisibleChanged(bool visible, int height)
{
    qDebug() << "sendVisibleChanged! visible = " << visible << " height = " << height;

    KeyboardSp &instance = KeyboardSp::instance();

    if (qApp) {
        if (instance._height != height) {
            instance._height = height;
            emit instance.heightChanged(instance._height);
        }

        if (instance._visible != visible || instance._height != height) {
            instance._visible = visible;
            emit instance.visibleChanged(instance._visible);
        }
    }
}

//------------------------------------------------------------------------------
sp::KeyboardSp::KeyboardSp()
    : QObject()
{
#if defined(Q_OS_IOS)
    [[KeyboardNotificationIos alloc] initNotifications];
#endif
}

//------------------------------------------------------------------------------
void sp::KeyboardSp::show() {
    #ifdef Q_OS_ANDROID
        QAndroidJniObject activity = QtAndroid::androidActivity();
        activity.callMethod<void>("showKeyboard");
    #endif
}

//------------------------------------------------------------------------------
void sp::KeyboardSp::hide() {
    #ifdef Q_OS_ANDROID
        QAndroidJniObject activity = QtAndroid::androidActivity();
        activity.callMethod<void>("hideKeyboard");
    #endif
}
