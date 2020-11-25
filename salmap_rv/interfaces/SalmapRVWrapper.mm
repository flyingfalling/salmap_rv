
#import <opencv2/opencv.hpp>
#import <opencv2/imgcodecs/ios.h>

#include <salmap_rv/include/salmap_rv_interface.hpp>

#import "SalmapRVWrapper.h"



#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@implementation SalmapRVWrapper

- (void*)salmap_ptr_init_wrap: (NSString*)_paramset_fname :(double)input_dva_per_pix :(long)dt_nsec :(long)nthreads
{
	return salmap_ptr_init( [_paramset_fname cStringUsingEncoding:NSUTF8StringEncoding], input_dva_per_pix, dt_nsec, nthreads );
}

- (void)salmap_ptr_uninit_wrap: (void*)_salmap
{
	salmap_ptr_uninit( _salmap );
}

- (long)salmap_update_wrap:  (void*)_salmap
{
	return salmap_update( _salmap );
}


- (void)salmap_add_input_wrap: (void*)_salmap :(NSString*)_inputmapname :(UIImage*)inputmat
{
	cv::Mat cvinput;
	UIImageToMat(inputmat, cvinput, true);
    
	
	cv::Mat cvinputrgb;
	
	cv::cvtColor(cvinput, cvinputrgb, cv::COLOR_RGBA2RGB);
	
    NSLog(@"REV: OBJC Will next call add_input (input is %d %d)", cvinputrgb.size().width, cvinputrgb.size().height);
    
	salmap_add_input( _salmap, [_inputmapname cStringUsingEncoding:NSUTF8StringEncoding], cvinputrgb );
    
    NSLog(@"REV: OBJC **FINISHED** add_input (input was %d %d)", cvinputrgb.size().width, cvinputrgb.size().height);
}

- (UIImage*)salmap_get_map_now_nickname_wrap: (void*)_salmap :(NSString*)filtername :(NSString*)mapname
{
	cv::Mat result = salmap_get_map_now_nickname( _salmap, [filtername cStringUsingEncoding:NSUTF8StringEncoding],  [mapname cStringUsingEncoding:NSUTF8StringEncoding] );
	return MatToUIImage(result);
}

- (UIImage*) salmap_get_map_pretty_wrap: (void*) _salmap :(NSString*) filtername :(NSString*)mapname :(NSString*)overlaywithinputmap :(double)alpha :(long)resize_wid_pix
{
	cv::Mat result = salmap_get_map_pretty( _salmap, [filtername cStringUsingEncoding:NSUTF8StringEncoding], [mapname cStringUsingEncoding:NSUTF8StringEncoding], [overlaywithinputmap cStringUsingEncoding:NSUTF8StringEncoding], alpha, resize_wid_pix );

	return MatToUIImage(result);
}


@end
