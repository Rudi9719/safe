//
//  SFXWelcomeWindowController.h
//  Safe
//
//  Created by Rian Hunter on 1/3/14.
//  Copyright (c) 2014 Rian Hunter. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef enum {
    WELCOME_WINDOW_NONE,
    WELCOME_WINDOW_CREATE,
    WELCOME_WINDOW_BUTTON_0 = WELCOME_WINDOW_CREATE,
    WELCOME_WINDOW_MOUNT,
    WELCOME_WINDOW_BUTTON_1 = WELCOME_WINDOW_MOUNT,
} welcome_window_action_t;

@class SFXWelcomeWindowController;

typedef void (^WelcomeWindowDoneBlock)(SFXWelcomeWindowController *, welcome_window_action_t);

@protocol SFXWelcomeWindowControllerDelegate <NSObject>

- (void)welcomeWindowDone:(id)sender withAction:(welcome_window_action_t)action;

@end

@interface SFXWelcomeWindowController : NSWindowController <NSWindowDelegate>

@property (nonatomic, strong) WelcomeWindowDoneBlock block;
@property (weak) IBOutlet NSButton *button1;
@property (weak) IBOutlet NSButton *button0;
@property (weak) IBOutlet NSTextField *messageTextField;

- (id)initWithDelegate:(NSObject <SFXWelcomeWindowControllerDelegate> *)delegate;
- (id)initWithBlock:(WelcomeWindowDoneBlock)block
              title:(NSString *)title
            message:(NSString *)message
       button0Title:(NSString *)button0Title
       button1Title:(NSString *)button1Title;

- (IBAction)createNewSafe:(id)sender;
- (IBAction)mountExistingSafe:(id)sender;

@end
