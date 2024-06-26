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
#import <AppKit/AppKit.h>

@class GVExportViewController;
@class GVZGraph;

@interface GVDocument : NSDocument
{
	GVExportViewController *_exporter;
	GVZGraph *_graph;
}

@property(readonly) GVZGraph *graph;

- (id)init;

- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError;
- (BOOL)writeToURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError;

- (void)makeWindowControllers;

- (void)setPrintInfo:(NSPrintInfo *)printInfo;

- (IBAction)exportDocument:(id)sender;
- (void)exporterDidEnd:(GVExportViewController *)exporter;

- (void)fileDidChange:(NSString *)path;
- (void)graphDidChange:(NSNotification *)notification;

- (void)dealloc;

@end
