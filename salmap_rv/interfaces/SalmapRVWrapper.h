#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface SalmapRVWrapper : NSObject

- (UIImage *)salmap_get_map_pretty_wrap: (void *)_salmap :(NSString *)filtername :(NSString *)mapname :(NSString *)overlaywithinputmap :(double)alpha :(long)resize_wid_pix;

- (void *)salmap_ptr_init_wrap: (NSString *)_paramset_fname :(double)input_dva_per_pix :(long)dt_nsec :(long)nthreads;

- (void)salmap_ptr_uninit_wrap: (void *)_salmap;

- (long)salmap_update_wrap:  (void *)_salmap;

- (void)salmap_add_input_wrap: (void *)_salmap :(NSString *)_inputmapname :(UIImage *)inputmat;

- (UIImage *)salmap_get_map_now_nickname_wrap: (void *)_salmap :(NSString *)filtername :(NSString *)mapname;

@end
