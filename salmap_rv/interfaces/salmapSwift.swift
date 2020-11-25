







import UIKit

class salmapSwift {
    var salmap_ptr : UnsafeMutableRawPointer;
    var wrapper : SalmapRVWrapper;
    
    init() {
        self.wrapper = SalmapRVWrapper();
        
        let paramfilename = "myparams.params";
        let paramfilepath = Bundle.main.resourcePath! + "/" + paramfilename;
        let dvaperpix=0.05;
        let dt_nsec=40000000;
        let nthreads=1;
        self.salmap_ptr = self.wrapper.salmap_ptr_init_wrap( paramfilepath, dvaperpix, dt_nsec, nthreads );
    }
    
    
    deinit {
        
        self.wrapper.salmap_ptr_uninit_wrap( salmap_ptr );
    }
    
    func input_update_retrieve( input: UIImage ) -> UIImage?
    {
        self.wrapper.salmap_add_input_wrap(self.salmap_ptr, "bgr", input);
        self.wrapper.salmap_update_wrap(self.salmap_ptr);
        let returned = self.wrapper.salmap_get_map_pretty_wrap(salmap_ptr, "finavg", "output", "bgr", 0.2, 500);
        return returned;
    }
}
