import os
import zipfile
import shutil


def unzip(zip_file, dest_dir):
    '''Unzip a file to a destination directory and delete the zip file.
    '''
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)
    with zipfile.ZipFile(zip_file, 'r') as z:
        z.extractall(dest_dir)
    # os.remove(zip_file)


def compile_java(src_path, class_path, java_files):
    '''Compile java files in src_path and output to class_path.
    '''
    cmd = 'javac -encoding UTF-8 -d %s -cp %s %s' % (class_path,
                                                     src_path, ' '.join(java_files))
    os.system(cmd)


def create_MANIFEST(class_path, main_class):
    '''Create a MANIFEST file.
    '''
    manifest = os.path.join(class_path, 'MY_MANIFEST.MF')
    with open(manifest, 'w') as f:
        f.write('Manifest-Version: 1.0\n')
        f.write('Created-By: Red\n')
        f.write('Main-Class: %s\n\n' % main_class)
    return manifest


def make_jar(src_path, jar_file, manifest='MY_MANIFEST.MF'):
    '''Create a jar file from a class path.
    '''
    cmd = 'jar cvfm %s %s -C %s .' % (jar_file, manifest, src_path)
    os.system(cmd)


def auto_jar(zip_path):

    if not os.path.exists('temp'):
        os.mkdir('temp')
    if not os.path.exists('jar'):
        os.mkdir('jar')

    for zip_file in os.listdir(zip_path):
        project_path = os.path.join('temp', zip_file[:-4])
        class_path = os.path.join(project_path, 'classes')
        if not os.path.exists(class_path):
            os.makedirs(class_path)
        src_path = 'unknown'
        main_class = 'unknown'
        java_files = []

        unzip(os.path.join(zip_path, zip_file), project_path)
        for dirpath, dirnames, filenames in os.walk(project_path):
            for filename in filenames:
                if filename.endswith('.java'):
                    java_file = os.path.join(dirpath, filename)
                    with open(java_file, 'r', encoding='utf8') as f:
                        if 'public static void main' in f.read():
                            src_path = dirpath
                            main_class = filename[:-5]
                    java_files.append(java_file)

        if main_class == 'unknown':
            print('No main class found in %s' % zip_file)
            continue

        compile_java(src_path, class_path, java_files)
        manifest = create_MANIFEST(class_path, main_class)
        make_jar(class_path, os.path.join(
            'jar', zip_file[:-4] + '.jar'), manifest)

    shutil.rmtree('temp')


if __name__ == '__main__':
    zip_path = input('Enter the path to the zip files: ')
    auto_jar(zip_path)
