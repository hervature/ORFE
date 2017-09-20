//
//  ViewController.swift
//  ORF525 Project
//
//  Created by Zachary Moore on 2017-05-13.
//  Copyright © 2017 Zachary Hervieux-Moore. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    var lastPoint = CGPoint.zero
    var red: CGFloat = 0.0
    var green: CGFloat = 0.0
    var blue: CGFloat = 0.0
    var brush: CGFloat = 5.0
    let opacity: CGFloat = 1.0
    var swiped: Bool = false
    var currentImage: Int = 1
    let imagesPath = Bundle.main.resourcePath! + "/Images"
    var imagesArray = [String]()
    
    @IBOutlet weak var mainImage: UIImageView!
    @IBOutlet weak var tempImage: UIImageView!
    @IBOutlet weak var equationImage: UIImageView!
    @IBOutlet weak var label: UILabel!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        
        // Look up current image
        let documentsDirectory = try! FileManager.default.url(for: .documentDirectory, in: .userDomainMask, appropriateFor: nil, create: true)
        let doneImages = try! FileManager.default.contentsOfDirectory(atPath: documentsDirectory.path)
        
        do {
            imagesArray = try FileManager.default.contentsOfDirectory(atPath: imagesPath)
        } catch {
            print(error)
        }
        
        if doneImages.count > 1 {
            currentImage = imagesArray.index(of: doneImages[doneImages.count-1])! + 2
        } else {
            currentImage = 1
        }
        
        let filename = imagesPath + "/" + imagesArray[currentImage-1]
        var image = UIImage(contentsOfFile: filename)
        let cgImage = image?.cgImage
        let toRect = CGRect(x: 0, y: 349, width: 1654, height: 200)
        let croppedCGImage = cgImage?.cropping(to: toRect)
        image = UIImage(cgImage: croppedCGImage!)
        equationImage.image = image
        label.text = String(currentImage)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func deletePressed(_ sender: UIButton) {
        mainImage.image = nil
    }
    
    @IBAction func submitPressed(_ sender: UIButton) {
        
        // Save image
        let imageData = UIImagePNGRepresentation(mainImage.image!)!
        let documentsDirectoryURL = try! FileManager.default.url(for: .documentDirectory, in: .userDomainMask, appropriateFor: nil, create: true)
        // create a name for your image
        let fileURL = documentsDirectoryURL.appendingPathComponent(imagesArray[currentImage-1])
        
        try! imageData.write(to: fileURL)

        // Load next equation
        currentImage = currentImage + 1
        
        let filename = imagesPath + "/" + imagesArray[currentImage-1]
        var image = UIImage(contentsOfFile: filename)
        let cgImage = image?.cgImage
        let toRect = CGRect(x: 0, y: 349, width: 1654, height: 200)
        let croppedCGImage = cgImage?.cropping(to: toRect)
        image = UIImage(cgImage: croppedCGImage!)
        equationImage.image = image
        label.text = String(currentImage)
        
        // Erase input area
        mainImage.image = nil
    }
    
    @IBAction func erasePressed(_ sender: Any) {
        red = 255.0
        green = 255.0
        blue = 255.0
        brush = 20.0
    }
    
    @IBAction func writePressed(_ sender: Any) {
        red = 0.0
        green = 0.0
        blue = 0.0
        brush = 5.0
    }
    
    @IBAction func skipPressed(_ sender: Any) {
        // Load next equation
        currentImage = currentImage + 1
        
        let filename = imagesPath + "/" + imagesArray[currentImage-1]
        var image = UIImage(contentsOfFile: filename)
        let cgImage = image?.cgImage
        let toRect = CGRect(x: 0, y: 349, width: 1654, height: 200)
        let croppedCGImage = cgImage?.cropping(to: toRect)
        image = UIImage(cgImage: croppedCGImage!)
        equationImage.image = image
        label.text = String(currentImage)
        
        // Erase input area
        mainImage.image = nil
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        swiped = false
        let touch = touches.first!
        if tempImage.frame.contains(touch.location(in: view)) {
            lastPoint = touch.location(in: tempImage)
        }
    }
    
    func drawLineFrom(fromPoint: CGPoint, toPoint: CGPoint) {
        
        // 1
        UIGraphicsBeginImageContext(tempImage.frame.size)
        let context = UIGraphicsGetCurrentContext()
        tempImage.image?.draw(in: CGRect(x: 0, y: 0, width: tempImage.frame.size.width, height: tempImage.frame.size.height))
        
        // 2
        context?.move(to: CGPoint(x: fromPoint.x, y: fromPoint.y))
        context?.addLine(to: CGPoint(x: toPoint.x, y: toPoint.y))
        
        // 3
        context?.setLineCap(CGLineCap.round)
        context?.setLineWidth(brush)
        context?.setStrokeColor(red: red, green: green, blue: blue, alpha: 1.0)
        context?.setBlendMode(CGBlendMode.normal)
        
        // 4
        context?.strokePath()
        
        // 5
        tempImage.image = UIGraphicsGetImageFromCurrentImageContext()
        tempImage.alpha = opacity
        UIGraphicsEndImageContext()
        
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        // 6
        swiped = true
        let touch = touches.first!
        
        if tempImage.frame.contains(touch.location(in: view)) {
            let currentPoint = touch.location(in: tempImage)
            drawLineFrom(fromPoint: lastPoint, toPoint: currentPoint)
            
            // 7
            lastPoint = currentPoint
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        
        if !swiped {
            // draw a single point
            drawLineFrom(fromPoint: lastPoint, toPoint: lastPoint)
        }
        
        // Merge tempImageView into mainImageView
        UIGraphicsBeginImageContext(mainImage.frame.size)
        mainImage.image?.draw(in: CGRect(x: 0, y: 0, width: tempImage.frame.size.width, height: tempImage.frame.size.height), blendMode: CGBlendMode.normal, alpha: 1.0)
        tempImage.image?.draw(in: CGRect(x: 0, y: 0, width: tempImage.frame.size.width, height: tempImage.frame.size.height), blendMode: CGBlendMode.normal, alpha: opacity)
        mainImage.image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        
        tempImage.image = nil
    }
}

