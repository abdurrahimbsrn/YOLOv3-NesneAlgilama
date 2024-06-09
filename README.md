YOLOv3 Nesne Algılama

Bu C++ programı, YOLOv3 (You Only Look Once version 3) modelini kullanarak bir görüntüdeki nesneleri algılamak için kullanılır. 
YOLOv3, gerçek zamanlı nesne algılama için kullanılan bir derin öğrenme modelidir.


Kullanılan Teknolojiler ve Kütüphaneler:

OpenCV: 
Görüntü işleme ve nesne algılama işlemleri için kullanılan bir kütüphane.

YOLOv3: Nesne algılama için yaygın olarak kullanılan bir derin öğrenme modeli.

Kullanım Adımları: Model ve Sınıf Dosyalarını Yükleme: YOLOv3 modeli (yolov3.cfg ve yolov3.weights) ve sınıf isimleri (coco.names) dosyaları belirtilen dosya yollarından yüklenir.


Görüntüyü Yükleme ve İşleme: 

İşlem yapılacak olan görüntü, belirtilen dosya yolundan yüklenir ve YOLOv3 modeline uygun formata dönüştürülür.


Nesne Algılama ve Sonuçların İşlenmesi: 

YOLOv3 modeli ile görüntü üzerinde nesne algılama yapılır. Algılama sonuçları işlenir ve en iyi tahminler belirlenir.


Non-Maximum Suppression (NMS) Uygulama:

Çakışan kutuları ve düşük güven skorlarına sahip olanları filtrelemek için NMS uygulanır.


Sonuçları Görselleştirme: 

Elde edilen en iyi tahminler, görüntü üzerine çizilerek görselleştirilir.


Kullanım:

modelConfiguration, modelWeights, ve classesFile değişkenlerini, kullanmak istediğiniz YOLOv3 modeli ve sınıf dosyalarının dosya yollarıyla güncelleyin.

İşlem yapmak istediğiniz görüntünün dosya yolunu frame değişkenine atanmış olan imread fonksiyonunun içine belirtin.

Kodu derleyip çalıştırın. Program, algılanan nesneleri konsola yazdıracak ve görüntüyü görsel olarak gösterecektir.


Örnek Çıktı:

Detected: cat at (100, 120, 150, 200) with confidence: 0.85

Detected: dog at (200, 150, 180, 220) with confidence: 0.92
