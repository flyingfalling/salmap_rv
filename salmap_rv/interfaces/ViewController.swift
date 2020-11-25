







import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var salOverlaid: UIImageView!
    var ss : salmapSwift = salmapSwift();
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        let bearpic = UIImage( named: "bear" )
        let salbear = self.ss.input_update_retrieve(input: bearpic!)
        
        salOverlaid.image = salbear;
        
    }


}

