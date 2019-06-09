import os
import zipfile

def zip_file(path, ziph):
    ziph.write(path)
    
def zip_dir(path, ziph):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip_file(os.path.join(root, file), ziph)
    

if __name__ == "__main__":
    ziph = zipfile.ZipFile("nia.zip", "w", zipfile.ZIP_DEFLATED)
    zip_file("build/libnia.dll", ziph)
    zip_dir("include", ziph)
    zip_dir("docs", ziph)
    ziph.close()