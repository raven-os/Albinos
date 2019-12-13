import setuptools

with open("README.md", "r") as f:
    long_description = f.read()

setuptools.setup(
    name="raven-os_albinos",
    version="0.0.1",
    author="Arthur Cros",
    author_email="arthur.cros@etna.io",
    description="Basic API to interact with albinos daemon",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/raven-os/albinos",
    packages=setuptools.find_namespace_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
    ],
)
