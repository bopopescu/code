//
//  ImageStore.swift
//  WorldTrotter
//
//  Created by Bart Bartel on 11/5/16.
//  Copyright © 2016 Bart Bartel. All rights reserved.
//

import UIKit

class ImageStore: NSObject {
    let cache = NSCache<AnyObject, AnyObject>()
    
    func setImage(image: UIImage, forKey key: String) {
        cache.setObject(image, forKey: key as AnyObject)
        
        // create full URL for image
        //let imageURL = imageForKey(key: key)
        
        // turn image into JPEG data
        //if let data = UIImageJPEGRepresentation(image, 0.5) {
        //    // write it to full URL
        //    data.write(to: imageURL, options: true)
        //}
        
    }
    
    func imageForKey(key: String) -> UIImage? {
        return cache.object(forKey: key as AnyObject) as? UIImage
    }
    
    func deleteImageForKey(key: String) {
        cache.removeObject(forKey: key as AnyObject)
    }
    
}
