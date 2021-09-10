//
//  PipesTestWrapper.m
//  pipes_gui
//
//  Created by admin on 10.09.2021.
//

#import <Foundation/Foundation.h>

#import "PipesTestWrapper.h"
#import "cppmain.hpp"
#import <string>

@implementation PipesTestWrapper
- (NSString *) PipesTest_run{
    PipesTest PipesTest;
    std::string helloWorldMessage = PipesTest.PipesTest_run();
    return [NSString
            stringWithCString:helloWorldMessage.c_str()
            encoding:NSUTF8StringEncoding];
}
@end
