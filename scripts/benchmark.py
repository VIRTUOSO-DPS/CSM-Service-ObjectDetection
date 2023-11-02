import requests
import time
import argparse
import json


def benchmark_post_requests(url, image_path, num_requests):
    with open(image_path, "rb") as image_file:
        image_bytes = image_file.read()

    total_response_time = 0

    print("Running...")

    for i in range(num_requests):
        print(f"\t> Request {i}")

        start_time = time.time()
        response = requests.post(
            url, data=image_bytes, headers={"Content-Type": "image/jpeg"}
        )
        end_time = time.time()

        delta_time = end_time - start_time

        response_json = response.json()
        print(f"\t< Response ({delta_time}s):\n{response_json}")

        total_response_time += delta_time

    average_time_per_request = total_response_time / num_requests

    print(f"Number of requests: {num_requests}")
    print(f"Average time per request: {average_time_per_request:.4f} seconds")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Image POST Request Benchmark")
    parser.add_argument(
        "--url", type=str, required=True, help="URL to send POST requests to"
    )
    parser.add_argument(
        "--image", type=str, required=True, help="Path to the image file"
    )
    parser.add_argument(
        "--requests",
        type=int,
        default=100,
        help="Number of requests to send (default: 100)",
    )

    args = parser.parse_args()

    benchmark_post_requests(args.url, args.image, args.requests)
