/*************************************************************************
 * Copyright (c) 2011 AT&T Intellectual Property 
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors: Details at http://www.graphviz.org/
 *************************************************************************/

#import "GVExportViewController.h"
#import "GVZGraph.h"

static NSMutableArray *_formatRenders = nil;

@implementation GVExportViewController

@synthesize URL = _url;
@synthesize render = _render;

+ (void)initialize
{
	if (!_formatRenders) {
		_formatRenders = [[NSMutableArray alloc] init];
		
		NSString *lastFormat = nil;
		NSMutableArray *lastRenders = nil;
		for (NSString *device in [GVZGraph pluginsWithAPI:API_device]) {
			NSArray *deviceComponents = [device componentsSeparatedByString:@":"];
			NSUInteger componentCount = [deviceComponents count];
			
			if (componentCount > 0) {
				NSString *format = [deviceComponents objectAtIndex:0];
				if (![lastFormat isEqualToString:format]) {
					lastFormat = format;
					lastRenders = [NSMutableArray array];
					[_formatRenders addObject:[NSDictionary dictionaryWithObjectsAndKeys:lastFormat, @"format", lastRenders, @"renders", nil]];
				}
			}
			
			if (componentCount > 1)
				[lastRenders addObject:[deviceComponents objectAtIndex:1]];
		}
	}
}

- (id)init
{
	if (self = [super initWithNibName:@"Export" bundle:nil]) {
		_panel = nil;
		_url = nil;
		
		_formatRender = nil;
		_render = nil;
		for (NSDictionary *formatRender in _formatRenders)
			if ([[formatRender objectForKey:@"format"] isEqualToString:@"pdf"]) {
				_formatRender = [formatRender retain];
				if ([[formatRender objectForKey:@"renders"] containsObject:@"quartz"])
					_render = @"quartz";
				break;
			}
	}
	return self;
}

- (NSArray *)formatRenders
{
	return _formatRenders;
}

- (NSString *)device
{
	NSString *format = [_formatRender objectForKey:@"format"];
	return _render ? [NSString stringWithFormat:@"%@:%@", format, _render] : format;
}

- (NSDictionary *)formatRender
{
	return _formatRender;
}

- (void)setFormatRender:(NSDictionary *)formatRender
{
	if (_formatRender != formatRender) {
		[_formatRender release];
		_formatRender = [formatRender retain];
		
		/* force save panel to use this file type */
		[_panel setAllowedFileTypes:[NSArray arrayWithObject:[_formatRender objectForKey:@"format"]]];
		
		/* remove existing render if it's not compatible with format */
		if (![[_formatRender objectForKey:@"renders"] containsObject:_render])
			[self setRender:nil];
	}
}

- (void)beginSheetModalForWindow:(NSWindow *)window modalDelegate:(id)modalDelegate didEndSelector:(SEL)selector
{
	/* remember to invoke end selector on the modal delegate */
	NSInvocation *endInvocation = [NSInvocation invocationWithMethodSignature:[modalDelegate methodSignatureForSelector:selector]];
	[endInvocation setTarget:modalDelegate];
	[endInvocation setSelector:selector];
	[endInvocation setArgument:&self atIndex:2];
	[endInvocation retain];

	_panel = [NSSavePanel savePanel];
	[_panel setAccessoryView:[self view]];
	[_panel setAllowedFileTypes:[NSArray arrayWithObject:[_formatRender objectForKey:@"format"]]];
	[_panel setDirectoryURL:[_url URLByDeletingLastPathComponent]];
	[_panel setNameFieldStringValue:[_url lastPathComponent]];
	[_panel beginSheetModalForWindow:window completionHandler:^(NSInteger result) {
		if (result == NSOKButton) {
			NSURL *url = [_panel URL];
			if (_url != url) {
				[_url release];
				_url = [url retain];
			}

			/* invoke the end selector on the modal delegate */
			[endInvocation invoke];
		}

		[endInvocation release];
		[_panel setAccessoryView:nil];
		_panel = nil;
	}];
}

- (void)dealloc
{
	[_panel release];
	[_url release];
	[_formatRender release];
	[_render release];
	[super dealloc];
}

@end
