
/**
 * OpenEmulator
 * Mac OS X Document Controller
 * (C) 2009-2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls all emulations.
 */

#import <Cocoa/Cocoa.h>

#import "InspectorWindowController.h"
#import "AudioControlsWindowController.h"
#import "TemplateChooserWindowController.h"

@class AudioControlsWindowController;
@class TemplateChooserWindowController;

@interface DocumentController : NSDocumentController {
	IBOutlet InspectorWindowController *fInspectorWindowController;
	IBOutlet AudioControlsWindowController *fAudioControlsWindowController;
	IBOutlet TemplateChooserWindowController *fTemplateChooserWindowController;
	
	NSArray *diskImagePathExtensions;
	NSArray *audioPathExtensions;
	
	void *oePortAudio;
	
	int disableMenuBarCount;
}

- (NSArray *)diskImagePathExtensions;
- (NSArray *)audioPathExtensions;
- (void *)oePortAudio;

- (void)toggleInspector:(id)sender;
- (void)toggleAudioControls:(id)sender;
- (IBAction)newDocumentFromTemplateChooser:(id)sender;

- (id)openUntitledDocumentWithTemplateURL:(NSURL *)absoluteURL
								  display:(BOOL)displayDocument
									error:(NSError **)outError;
- (id)makeUntitledDocumentWithTemplateURL:(NSURL *)absoluteURL
									error:(NSError **)outError;

- (BOOL)addEmulation:(void *)emulation;
- (void)removeEmulation:(void *)emulation;

- (void)startPlaying:(NSString *)path;
- (void)stopPlaying;
- (BOOL)isPlaying;
- (float)getPlayTime;
- (float)getPlayDuration;
- (void)startRecording:(NSString *)path;
- (void)stopRecording;
- (BOOL)isRecording;
- (float)getRecordingTime;
- (long long)getRecordingSize;

- (void)disableMenuBar;
- (void)enableMenuBar;

- (IBAction)openHomepage:(id)sender;
- (IBAction)openForums:(id)sender;
- (IBAction)openDevelopment:(id)sender;
- (IBAction)openDonate:(id)sender;

@end
