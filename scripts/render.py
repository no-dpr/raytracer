import subprocess
from PIL import Image
from io import BytesIO

RENDER_NAME = "render"
RENDER_PATH = f"renders/{RENDER_NAME}.png"

if __name__=="__main__":
    result: bytes = bytes()
    try:
        subprocess.run(['cmake', '--build', 'build'], capture_output=False, text=True, check=True)
        process = subprocess.Popen(['build/Raytracer'],   
            stdout=subprocess.PIPE,       # capture stdout (std::cout)
            stderr=None,                  # let stderr (std::clog/cerr) go directly to terminal
        )

        result, _ = process.communicate()

    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        print(f"Stderr: {e.stderr}")
        raise e

    else:
        render = Image.open(BytesIO(result))
        render.save(RENDER_PATH, "PNG")