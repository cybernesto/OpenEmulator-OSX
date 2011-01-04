
/**
 * OpenEmulator
 * Mac OS X Chooser View Controller
 * (C) 2009-2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls a template chooser view.
 */

#import "TemplateChooserViewController.h"
#import "Document.h"
#import "ChooserItem.h"
#import "StringConversion.h"

#import "OEEDL.h"

#define USER_TEMPLATES_GROUP @"My Templates"

@implementation TemplateChooserViewController

- (void)loadGroups
{
	NSString *templatesPath = [[[NSBundle mainBundle] resourcePath]
							   stringByAppendingPathComponent:@"templates"];
	
	[groups removeAllObjects];
	
	// Find templates
	NSFileManager *fileManager = [NSFileManager defaultManager];
	NSArray *subpaths = [fileManager contentsOfDirectoryAtPath:templatesPath
														 error:nil];
	for (NSString *pathComponent in subpaths)
	{
		NSString *groupPath = [templatesPath stringByAppendingPathComponent:pathComponent];
		if ([self validTemplatesAtPath:groupPath])
			[groups addObject:pathComponent];
	}
	
	// Sort alphabetically
	[groups setArray:[groups sortedArrayUsingSelector:@selector(compare:)]];
	
	// Find user templates
	[items removeObjectForKey:NSLocalizedString(USER_TEMPLATES_GROUP,
												"Template Chooser.")];
	if ([self validTemplatesAtPath:USER_TEMPLATES_FOLDER])
		[groups addObject:NSLocalizedString(USER_TEMPLATES_GROUP,
											"Template Chooser.")];
}

- (void)loadItems
{
	if ([items objectForKey:selectedGroup])
		return;
	
	NSString *group = NSLocalizedString(USER_TEMPLATES_GROUP,
										"Template Chooser.");
	if ([selectedGroup compare:group] == NSOrderedSame)
		[self addGroupAtPath:USER_TEMPLATES_FOLDER
					 toGroup:group];
	else
	{
		NSString *templatesPath = [[[NSBundle mainBundle] resourcePath]
								   stringByAppendingPathComponent:@"templates"];
		NSString *groupPath = [templatesPath stringByAppendingPathComponent:selectedGroup];
		[self addGroupAtPath:groupPath
					 toGroup:selectedGroup];
	}
}

- (BOOL)validTemplatesAtPath:(NSString *)groupPath
{
	groupPath = [groupPath stringByExpandingTildeInPath];
	NSFileManager *fileManager = [NSFileManager defaultManager];
	BOOL isDirectory;
	if (![fileManager fileExistsAtPath:groupPath isDirectory:&isDirectory] ||
		!isDirectory)
		return NO;
	
	NSArray *subpaths = [fileManager contentsOfDirectoryAtPath:groupPath
														 error:nil];
	for (NSString *pathComponent in subpaths)
	{
		NSString *pathExtension = [[pathComponent pathExtension] lowercaseString];
		
		if (([pathExtension compare:@OE_PACKAGE_PATH_EXTENSION] == NSOrderedSame) ||
			([pathExtension compare:@OE_FILE_PATH_EXTENSION] == NSOrderedSame))
			return YES;
	}
	
	return NO;
}

- (void)addGroupAtPath:(NSString *)path
			   toGroup:(NSString *)group
{
	if (![items objectForKey:group])
		[items setObject:[NSMutableArray array] forKey:group];
	
	path = [path stringByExpandingTildeInPath];
	
	NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
	NSArray *pathContents = [[NSFileManager defaultManager]
							 contentsOfDirectoryAtPath:path
							 error:nil];
	for (NSString *edlFilename in pathContents)
	{
		NSString *edlPath = [path stringByAppendingPathComponent:edlFilename];
		
		NSString *pathExtension = [[edlPath pathExtension] lowercaseString];
		if (([pathExtension compare:@OE_PACKAGE_PATH_EXTENSION] != NSOrderedSame) &&
			([pathExtension compare:@OE_FILE_PATH_EXTENSION] != NSOrderedSame))
			continue;
		
		OEEDL edl;
		edl.open(getCPPString(edlPath));
		if (edl.isOpen())
		{
			OEHeaderInfo headerInfo = edl.getHeaderInfo();
			NSString *edlLabel = [edlFilename stringByDeletingPathExtension];
			NSString *edlImage = [resourcePath stringByAppendingPathComponent:
								  getNSString(headerInfo.image)];
			NSString *edlDescription = getNSString(headerInfo.description);
			
			ChooserItem *chooserItem = [[ChooserItem alloc] initWithLabel:edlLabel
																imagePath:edlImage
															  description:edlDescription
																  edlPath:edlPath
																	 data:nil];
			if (chooserItem)
			{
				[[items objectForKey:group] addObject:chooserItem];
				[chooserItem release];
			}
		}
	}
}

@end
