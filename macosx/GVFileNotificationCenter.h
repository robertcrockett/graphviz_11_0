/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at http://www.graphviz.org/
 *************************************************************************/

#import <Foundation/Foundation.h>

@interface GVFileNotificationCenter : NSObject
{
	CFFileDescriptorRef _queue;
	NSMutableSet *_records;
}

+ (void)initialize;
+ (id)defaultCenter;

- (id)init;

- (void)addObserver:(id)observer selector:(SEL)selector path:(NSString *)path;
- (void)removeObserver:(id)observer path:(NSString *)path;

@end
